import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.12

import Box2D 2.0
import Chronicles 1.0
import "../components"
import "../physics"

PhysicsItem {
    id: root

    property Character character
    property bool isPlayer: false
    property int itemType: character.itemType

    property real auraRadius: (character.auraRange + character.size.width / 2) * app.gridSize
    property real hitAttackRadius: root.width / 2 + 2 * app.gridSize // TODO: use range of current weapon
    property real hitAttackRadiusBase: root.width / 3

    antialiasing: app.antialiasing
    bodyType: character.movable ? character.bodyType : GameObject.BodyTypeStatic
    onPlayerAuraRangeChanged: character.playerVisible = playerAuraRange
    linearDamping: 1
    fixedRotation: true

    rotation: character.angle * 180 / Math.PI

    fixtures: [
        Circle {
            id: bodyCircle
            radius: root.width / 2
            density: 1
            friction: 0.0
            restitution: 0.0
            categories: character.categoryFlag
            collidesWith: character.collisionFlag

            onBeginContact: {
                if (!isPlayer)
                    return

                var target = other.getBody().target
                if (target.itemType) {
                    target.playerOnItem = true
                }
            }

            onEndContact: {
                if (!isPlayer)
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
                if (!isPlayer)
                    return

                var target = other.getBody().target
                if (target.itemType) {
                    target.playerOnItem = true
                }
            }

            onEndContact: {
                if (!isPlayer)
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
                if (!isPlayer)
                    return

                var target = other.getBody().target
                if (target.itemType) {
                    target.playerAuraRange = true
                }
            }

            onEndContact: {
                if (!isPlayer)
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
                    Game.world.performHitAttack(root.character, target.enemy, 10)
                }
            }
        }
    ]

    function getBulletX() {
        var centerX = root.x + root.width / 2
        return (centerX + (root.width / 2 + app.gridSize / 4 + app.gridSize / 2) * Math.cos(character.angle)) - app.gridSize / 4
    }

    function getBulletY() {
        var centerY = root.y + root.height / 2
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

        FlameItem {
            anchors.fill: parent
            enabled: false
        }


        Rectangle {
            id: frameWire
            color: "transparent";
            border.color: "red";
            border.width: 2;
            opacity: Game.debugging ? 0.5 : 0
        }

        ItemDescription {
            id: nameLabel
            anchors.bottom: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            //text: character ? character.name : ""
            text: character.name
            opacity: character ? (Game.debugging ? 0.5 : (root.character.playerFocus && !root.isPlayer ? 1 : 0)) : 0
        }

        RowLayout {
            id: healthIndicator
            height: app.gridSize / 6
            width: app.gridSize * 2
            anchors.bottom: nameLabel.top
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 0
            opacity: 0

            Behavior on opacity { NumberAnimation { duration: 200 } }

            Rectangle {
                color: "red"
                Layout.fillHeight: true
                Layout.preferredWidth: parent.width * character.healthPercentage / 100
            }

            Rectangle {
                color: "gray"
                Layout.fillHeight: true
                Layout.fillWidth: true
            }
        }

        Image {
            id: playerImage
            anchors.fill: frame
            source: character.heading === Character.HeadingRight ? dataDirectory + "/images/characters/player-male.png" : dataDirectory + "/images/characters/player-male-mirror.png"
            opacity: Game.debugging ? 0.5 : 1
        }
    }

    Timer {
        id: hitAttackTimer
        interval: 300 // TODO: depend on selected weapon
        repeat: false
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
        visible: root.isPlayer
        width: parent.width / 2 + character.auraRange * app.gridSize
        height: app.gridSize
        x: parent.width / 2
        y: parent.height / 2 - height / 2

        Image {
            id: directionIndicatorImage
            anchors.right: parent.right
            anchors.top: parent.top
            opacity: 0.4
            width: character.auraRange * app.gridSize
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
        border.color: "red"
        border.width: parent.width / 6
        opacity: 0
    }
}
