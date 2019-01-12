import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
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

    property bool fireArrow: false

    onXChanged: evaluateDistance()
    onYChanged: evaluateDistance()

    bullet: true
    fixedRotation: false
    bodyType: Body.Dynamic
    smooth: true

    fixtures: Circle {
        categories: GameObject.PhysicsBullet
        collidesWith: GameObject.PhysicsCharacter | GameObject.PhysicsEnemy | GameObject.PhysicsStaticItem | GameObject.PhysicsBullet
        radius: root.width / 2
        density: 1
        friction: 0
        restitution: 0
        onBeginContact: {
            var target = other.getBody().target
            var victim = null
            if (target.gameItem) {
                // If we have a collision with an item
                switch (target.itemType) {
                case GameItem.TypeChest:
                    console.log("Bullet collision with chest")
                    root.body.linearVelocity = Qt.point(0, 0)
                    dissapearAnimation.start()
                    break;
                case GameItem.TypeTree:
                    console.log("Bullet collision with tree")
                    root.body.linearVelocity = Qt.point(0, 0)
                    dissapearAnimation.start()
                    break;

                    // TODO: colides with burning item

                default:
                    break
                }
                return
            } else {
                root.body.linearVelocity = Qt.point(0, 0)
                dissapearAnimation.start()
            }

            if (target.enemy) {
                victim = target.enemy
                if (root.fireArrow) target.inflame()
            } else if (target.character) {
                victim = target.character
                if (root.fireArrow) target.inflame()
            } else {
                return
            }

            console.log("Bullet collision " + root.shooter + " --> " + victim )
            Game.world.performShootImpact(root.shooter, victim, damage)
            root.destroy()
        }
    }

    Image {
        id: arrowImage
        anchors.fill: parent
        source: dataDirectory + "/images/items/firearms/arrow.png"
    }

    FlameItem {
        id: flameItem
        anchors.fill: parent
        rotation: angle = -90
        angleVariation: 10
        enabled: root.fireArrow
    }

    PropertyAnimation {
        id: dissapearAnimation
        target: root
        property: "opacity"
        to: 0
        duration: 1000
        loops: 1

        onRunningChanged: {
            if (running) {
                flameItem.angleVariation = 360
                flameItem.turbulence = true
            } else {
                root.destroy()
            }
        }
    }

    function evaluateDistance() {
        if (body.linearVelocity == Qt.point(0, 0) || dissapearAnimation.running)
            return

        var distance = Math.sqrt(Math.pow(root.x - root.startPositionX, 2) + Math.pow(root.y - root.startPositionY, 2))
        //console.log("Bullet distance traveld", distance, "/", root.maxDistance)
        if (distance >= maxDistance) {
            console.log("Bullet reached max distance", distance, ">=", root.maxDistance)
            root.body.linearVelocity = Qt.point(0, 0)
            dissapearAnimation.start()
        }
    }
}

