import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.0

import Chronicles 1.0
import Box2D 2.0

import "../components"
import "../physics"

PhysicsItem {
    id: root

    property Character character
    property int itemType: character ? character.itemType : GameItem.TypeNone

    property real auraRadius: character ? (character.auraRange + character.size.width / 2) * app.gridSize : app.gridSize
    property real hitAttackRadius: root.width / 2 + 2 * app.gridSize // TODO: use range of current weapon
    property real hitAttackRadiusBase: root.width / 3

    property bool burning: false

    function inflame() {
        if (flameFadeOutAnimation.running)
            flameFadeOutAnimation.stop()

        flameItem.opacity = 1
        flameItem.visible = true
        buringTimer.restart()
        root.burning = true
    }

    antialiasing: app.antialiasing
    bodyType: character ? (character.movable ? character.bodyType : GameObject.BodyTypeStatic) : GameObject.BodyTypeStatic
    onPlayerAuraRangeChanged: if (character) character.playerVisible = playerAuraRange
    linearDamping: 10
    fixedRotation: true
    rotation: character ? character.angle * 180 / Math.PI : 0

    // Character connections
    Connections {
        id: characterConnections
        target: character
        onShoot: shoot()
    }

    // Character movement
    Connections {
        target: Game.engine
        onEnginePostTick: {
            var currentVelocity = body.linearVelocity
            var dvx = character.movementVector.x * app.gridSize - currentVelocity.x
            var dvy = character.movementVector.y * app.gridSize - currentVelocity.y
            body.applyLinearImpulse(Qt.point(body.getMass() * dvx, body.getMass() * dvy), body.getWorldCenter())
        }
    }

    fixtures: [
        Circle {
            id: bodyCircle
            radius: root.width / 2
            density: 1
            friction: 0.0
            restitution: 0.0
            categories: character ? character.categoryFlag : 0
            collidesWith: character ? character.collisionFlag : 0

            onBeginContact: {
                if (!character.isPlayer)
                    return

                var target = other.getBody().target
                if (target.itemType) {
                    target.playerOnItem = true
                }
            }

            onEndContact: {
                if (!character.isPlayer)
                    return

                var target = other.getBody().target
                if (target.itemType) {
                    target.playerOnItem = false
                }
            }
        },
        Circle {
            id: bodySensorCircle
            radius: root.width / 2
            density: 0.0
            friction: 0.0
            restitution: 0.0
            sensor: true
            categories: GameItem.PhysicsSensor
            collidesWith: GameItem.PhysicsAll

            onBeginContact: {
                if (!character.isPlayer)
                    return

                var target = other.getBody().target
                if (target.itemType) {
                    target.playerOnItem = true
                }
            }

            onEndContact: {
                if (!character.isPlayer)
                    return

                var target = other.getBody().target
                if (target.itemType) {
                    target.playerOnItem = false
                }
            }
        },
        Polygon {
            id: auraSensor
            sensor: true

            density: 0.0
            friction: 0.0
            restitution: 0.0
            categories: GameItem.PhysicsSensor
            collidesWith: GameItem.PhysicsAll

            vertices: [
                Qt.point(root.width / 2, root.height / 2),
                Qt.point(root.width / 2 + auraRadius * Math.cos(Math.PI / 4), root.height / 2 + auraRadius * Math.sin(Math.PI / 4)),
                Qt.point(root.width / 2 + auraRadius * Math.cos(Math.PI / 8), root.height / 2 + auraRadius * Math.sin(Math.PI / 8)),
                Qt.point(root.width / 2 + auraRadius * Math.cos(0), root.height / 2 + auraRadius * Math.sin(0)),
                Qt.point(root.width / 2 + auraRadius * Math.cos(-Math.PI / 8), root.height / 2 + auraRadius * Math.sin(-Math.PI / 8)),
                Qt.point(root.width / 2 + auraRadius * Math.cos(-Math.PI / 4), root.height / 2 + auraRadius * Math.sin(-Math.PI / 4))
            ]

            onBeginContact: {
                if (!character.isPlayer)
                    return

                var target = other.getBody().target
                if (target.itemType) {
                    target.playerAuraRange = true
                }
            }

            onEndContact: {
                if (!character.isPlayer)
                    return

                var target = other.getBody().target
                if (target.itemType) {
                    target.playerAuraRange = false
                }
            }
        },
        Polygon {
            id: hitAttackSensor
            sensor: true
            density: 0.0
            friction: 0.0
            restitution: 0.0
            categories: GameItem.PhysicsSensor
            collidesWith: GameItem.PhysicsAll

            property real currentRadius: hitAttackTimer.running ? root.hitAttackRadius : root.hitAttackRadiusBase

            vertices: [
                Qt.point(root.width / 2, root.height / 2),
                Qt.point(root.width / 2 + currentRadius * Math.cos(Math.PI / 8), root.height / 2 + currentRadius * Math.sin(Math.PI / 8)),
                Qt.point(root.width / 2 + currentRadius * Math.cos(0), root.height / 2 + currentRadius * Math.sin(0)),
                Qt.point(root.width / 2 + currentRadius * Math.cos(-Math.PI / 8), root.height / 2 + currentRadius * Math.sin(-Math.PI / 8)),
            ]

            onBeginContact: {
                if (currentRadius === root.hitAttackRadiusBase)
                    return

                var target = other.getBody().target
                if (target.itemType && target.enemy) {
                    //hitAttackTimer.restart()
                    Game.engine.performHitAttack(root.character, target.enemy, 10)
                }
            }
        }
    ]

    function getBulletXOffset() {
        var centerX = root.width / 2
        return (centerX + (root.width / 2 + app.gridSize / 4 + app.gridSize / 2) * Math.cos(character.angle)) - app.gridSize / 4
    }

    function getBulletYOffset() {
        var centerY = root.height / 2
        return (centerY + (root.height / 2 + app.gridSize / 4 + app.gridSize / 2) * Math.sin(character.angle)) - app.gridSize / 4
    }

    function getBulletAngle() {
        var angle = root.character.angle * 180 / Math.PI;
        return angle
    }

    Item {
        id: frame
        anchors.fill: parent;
        rotation: -root.rotation

        Rectangle {
            id: wireFrame
            anchors.fill: parent
            color: "gray";
            border.color: "white";
            border.width: app.borderWidth / 2
            opacity: root.itemDebugEnabled? 0.2 : 0
        }

        FlameItem {
            id: flameItem
            anchors.fill: parent
            enabled: root.burning

            Timer {
                id: buringTimer
                interval: 5000
                onTriggered: flameFadeOutAnimation.start()
            }

            PropertyAnimation {
                id: flameFadeOutAnimation
                duration: 2000
                target: flameItem
                property: "opacity"
                loops: 1
                to: 0
                onRunningChanged: {
                    if (!running) {
                        flameItem.visible = false
                        root.burning = false
                    }
                }
            }
        }

        ItemDescription {
            id: nameLabel
            anchors.bottom: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            text: character ? character.name : ""
            opacity: character ? (root.itemDebugEnabled ? 0.5 : (root.character.playerFocus && !character.isPlayer ? 1 : 0)) : 0
        }

        PercentageIndicator {
            id: healthIndicator
            height: app.gridSize / 6
            width: app.gridSize * 2
            anchors.bottom: nameLabel.top
            anchors.horizontalCenter: parent.horizontalCenter
            opacity: 0

            color: app.healthColor
            percentage: character ? character.healthPercentage : 0
        }

        Image {
            id: playerImage
            anchors.fill: frame
            source: character ? (character.heading === Character.HeadingRight ? dataDirectory + "/images/characters/player-male.png" : dataDirectory + "/images/characters/player-male-mirror.png") : ""
            opacity: root.itemDebugEnabled ? 0.5 : 1
        }
    }

    Timer {
        id: hitAttackTimer
        interval: 300 // TODO: depend on selected weapon
        repeat: false
    }

    Timer {
        id: burnDamageTimer
        interval: 1000
        running: root.burning
        repeat: true
        onTriggered: Game.engine.performBurnDamage(root.character, 2)
    }

    Connections {
        id: attackConnections
        target: root.character
        onHit: {
            if (hitAttackTimer.running)
                return

            console.log("Hit !!!!!")
            healthIndicator.opacity = 1
            healthIndicatorTimer.restart()
            hitAttackTimer.restart()
        }
    }

    // Health indicator
    Connections {
        id: healthIndicatorConnections
        target: root.character
        onDamaged: {
            healthIndicator.opacity = 1
            healthIndicatorTimer.restart()
            damageAnimation.restart()
        }

        onHealed: {
            healthIndicator.opacity = 1
            healthIndicatorTimer.restart()
        }

        onKilled: {
            healthIndicator.opacity = 0
            healthIndicatorTimer.stop()
        }

        onPlayerOnItemChanged: {
            healthIndicator.opacity = 1
            healthIndicatorTimer.restart()
        }

        onPlayerFocusChanged: {
            if (playerFocus) {
                healthIndicator.opacity = 1
                healthIndicatorTimer.restart()
            } if (!playerFocus) {
                healthIndicator.opacity = 0
                healthIndicatorTimer.stop()
            }
        }
    }

    Timer {
        id: healthIndicatorTimer
        interval: 3000
        repeat: false
        onTriggered: healthIndicator.opacity = 0
    }

    // Damage
    ParallelAnimation {
        id: damageAnimation

        ScaleAnimator {
            target: damageIndicator
            duration: 200
            from: 1.2
            to: 1.5
        }

        SequentialAnimation {

            NumberAnimation {
                target: damageIndicator
                properties: "opacity"
                duration: 100
                from: 0
                to: 0.5
            }

            NumberAnimation {
                target: damageIndicator
                properties: "opacity"
                duration:  100
                to: 0
            }
        }
    }

    Item {
        id: directionIndicator
        visible: character ? character.isPlayer : false
        width: root.auraRadius
        height: app.gridSize
        x: parent.width / 2
        y: parent.height / 2 - height / 2

        Image {
            id: directionIndicatorImage
            anchors.right: parent.right
            anchors.top: parent.top
            opacity: 0.4
            width: root.auraRadius
            height: parent.height
            source: dataDirectory + "/images/game/direction-indicator.svg"
        }

        Image {
            id: weaponImage
            anchors.left: directionIndicatorImage.left
            anchors.verticalCenter: directionIndicatorImage.verticalCenter
            rotation: 45
            width: app.gridSize * 1.5
            height: width
            visible: hitAttackTimer.running
            source: dataDirectory + "/images/items/weapons/sword.png"
        }
    }

    Rectangle {
        id: damageIndicator
        width: parent.width
        height: parent.height
        radius: parent.width / 2
        color: "transparent"
        border.color: app.healthColor
        border.width: parent.width / 8
        opacity: 0
    }

    // Move and shoot
    onXChanged: {
        if (!character)
            return

        character.position = Qt.point(x / app.gridSize, y / app.gridSize)
    }

    onYChanged: {
        if (!character)
            return

        character.position = Qt.point(x / app.gridSize, y / app.gridSize)
    }

    function shootBulletObject(bulletObject) {
        var bulletStartPoint = Qt.point(root.x + getBulletXOffset(),  root.y + getBulletYOffset())
        bulletObject.damage = root.character.firearm.damage
        bulletObject.width = app.gridSize / 2
        bulletObject.height = bulletObject.width
        bulletObject.rotation = getBulletAngle()
        bulletObject.x = bulletStartPoint.x
        bulletObject.y = bulletStartPoint.y
        bulletObject.z = Map.Layer2Normal
        bulletObject.startPositionX = bulletStartPoint.x
        bulletObject.startPositionY = bulletStartPoint.y
        bulletObject.shootRange = root.character.firearm.range
        bulletObject.fireArrow = debugControls.flamesEnabled
        var velocity = app.gridSize * 1.2
        bulletObject.body.linearVelocity = Qt.point(velocity * Math.cos(character.angle), velocity * Math.sin(character.angle))
    }

    function shoot() {
        if (!character.firearm) {
            console.log(character.name, "can not shoot. No firearm selected.")
            return;
        }

        console.log(character.name + " SHOOOT! using", character.firearm)
        var component = Qt.createComponent("BulletItem.qml");
        var bulletIncubator = component.incubateObject(worldItem, { shooter: root.character } )
        if (bulletIncubator && bulletIncubator.status !== Component.Ready) {
            bulletIncubator.onStatusChanged = function(status) {
                if (status === Component.Ready) {
                    shootBulletObject(bulletIncubator.object)
                }
            }
        } else {
            shootBulletObject(bulletIncubator.object)
        }
    }
}
