import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtQuick.Particles 2.0
import QtGraphicalEffects 1.12

import Box2D 2.0
import Chronicles 1.0

import "../components"
import "../physics"

PhysicsItem {
    id: root

    property Character shooter
    property int damage
    property int shootRange

    property real startPositionX
    property real startPositionY
    property real maxDistance: shootRange * app.gridSize

    property bool lightEnabled: false
    property bool burning: false

    onXChanged: evaluateDistance()
    onYChanged: {
        z = gameScene.calculateLayerValue(GameObject.LayerItems, y, height, worldItem.height)
        evaluateDistance()
    }

    onBurningChanged: {
        if (burning)
            lightEnabled = true
    }

    bullet: true
    fixedRotation: false
    bodyType: Body.Dynamic
    smooth: true

    fixtures: [
        Circle {
            categories: GameObject.PhysicsBullet
            collidesWith: GameObject.PhysicsCharacter |
                          GameObject.PhysicsEnemy |
                          GameObject.PhysicsStaticItem |
                          GameObject.PhysicsBodyHitbox |
                          GameObject.PhysicsFire
            radius: root.width / 2
            density: 20
            friction: 0
            restitution: 0
            onBeginContact: {
                var target = other.getBody().target
                var victim = null
                if (target.gameItem) {

                    // TODO: colides with fire item ignite arrow

                    // If we have a collision with an item
                    switch (target.itemType) {
                    case GameItem.TypeChest:
                        console.log("Arrow collision with chest")
                        root.body.linearVelocity = Qt.point(0, 0)
                        root.active = false
                        dissapearAnimation.start()
                        break;
                    case GameItem.TypeStatic:
                        console.log("Arrow collision with static item")
                        root.body.linearVelocity = Qt.point(0, 0)
                        root.active = false
                        dissapearAnimation.start()
                        break;
                    case GameItem.TypeBox:
                        console.log("Arrow collision with box")
                        root.body.linearVelocity = Qt.point(0, 0)
                        root.active = false
                        dissapearAnimation.start()
                        break;
                    case GameItem.TypeTree:
                        console.log("Arrow collision with tree")
                        root.body.linearVelocity = Qt.point(0, 0)
                        root.active = false
                        dissapearAnimation.start()
                        break;
                    default:
                        root.body.linearVelocity = Qt.point(0, 0)
                        root.active = false
                        dissapearAnimation.start()
                        break
                    }
                    return
                } else {
                    root.body.linearVelocity = Qt.point(0, 0)
                    dissapearAnimation.start()
                }

                if (target.enemy) {
                    victim = target.enemy
                    if (root.burning) target.inflame()
                } else if (target.character) {
                    victim = target.character
                    if (root.burning) target.inflame()
                } else {
                    return
                }

                //console.log("Arrow impact", root.shooter.name, " --> ", victim.name, "damage:", damage )
                Game.engine.performShootImpact(root.shooter, victim, damage)
                root.destroy()
            }
        },
        Circle {
            categories: GameObject.PhysicsFire
            collidesWith: GameObject.PhysicsFire
            radius: root.width / 2
            density: 0
            sensor: true
            friction: 0
            restitution: 0
            onBeginContact: {
                // Get fireitem
                var flameItem = other.getBody().target
                if (!flameItem.fireItem)
                    return

                if (root.burning && flameItem.fireItem.burning) {
                    console.log("Burning bullet contact --> burning fire | do nothing")
                } else if (!root.burning && flameItem.fireItem.burning) {
                    root.burning = true
                    console.log("Bullet contact --> burning fire  | ignite bullet")
                } else if (root.burning && !flameItem.fireItem.burning) {
                    console.log("Burning bullet contact --> off fire  | ignite fire")
                    flameItem.fireItem.burning = true
                } else if (!root.burning && !flameItem.fireItem.burning) {
                    console.log("Bullet contact --> off fire  | do nothing")
                }
            }
        }
    ]

    LightSource {
        id: lightSource
        size:  Qt.size(5, 5)
        position: Qt.point(root.x / app.gridSize - size.width / 2, root.y  / app.gridSize - size.height / 2)
        lightEnabled: root.lightEnabled
        color: "#ce6fb2cb"
        name: "Bullet light"

        Component.onCompleted: Game.engine.mapScene.registerTemporaryLightSource(lightSource)
        Component.onDestruction: Game.engine.mapScene.unregisterTemporaryLightSource(lightSource)
    }

    Image {
        id: arrowImage
        anchors.fill: parent
        source: dataDirectory + "/images/items/firearms/arrow.png"
    }

    Item {
        id: flameItem
        anchors.fill: parent
        rotation: -90

        property int angle: -90
        property int magnitude: 30
        property int angleVariation: 20

        ParticleSystem {
            anchors.fill: parent
            running: root.burning

            ImageParticle {
                groups: ["flame"]
                source: dataDirectory + "/images/game/glowdot.png"
                color: "#11ff400f"
                colorVariation: 0.2
            }

            Emitter {
                anchors.centerIn: parent
                group: "flame"
                emitRate: 200 * app.gameSpeedFactor
                lifeSpan: 2000 / app.gameSpeedFactor
                size: app.gridSize / 2
                endSize: app.gridSize / 2
                sizeVariation: app.gridSize / 2
                acceleration: PointDirection { y: -60 }
                velocity: AngleDirection {
                    angle: flameItem.angle
                    magnitude: flameItem.magnitude
                    angleVariation: flameItem.angleVariation
                    magnitudeVariation: 8
                }
            }
        }
    }

    ParallelAnimation {
        id: dissapearAnimation
        onRunningChanged: {
            if (running) {
                root.lightEnabled = false
                flameItem.angle = 180
                flameItem.angleVariation = 360
                flameItem.magnitude = 10
            } else {
                root.destroy()
            }
        }

        PropertyAnimation {
            target: root
            property: "opacity"
            to: 0
            duration: 1000
            easing.type: Easing.OutCubic
            loops: 1
        }
    }



    function evaluateDistance() {
        if (body.linearVelocity == Qt.point(0, 0) || dissapearAnimation.running)
            return

        var distance = Math.sqrt(Math.pow(root.x - root.startPositionX, 2) + Math.pow(root.y - root.startPositionY, 2))
        //console.log("Bullet distance traveld", distance, "/", root.maxDistance)
        if (distance >= maxDistance) {
            console.log("Arrow reached max distance", distance, ">=", root.maxDistance)
            root.body.linearVelocity = Qt.point(0, 0)
            dissapearAnimation.start()
        }
    }
}

