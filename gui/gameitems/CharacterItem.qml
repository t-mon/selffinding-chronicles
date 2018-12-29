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

    property Character character: null
    property bool isPlayer: false
    property int itemType: character.itemType

    property real auraRadius: (character.auraRange + character.size.width / 2) * app.gridSize
    property real hitAttackRadius: character.size.width / 3 * app.gridSize

    antialiasing: app.antialiasing
    bodyType: character.movable ? Body.Dynamic : Body.Static
    onPlayerAuraRangeChanged: character.playerVisible = playerAuraRange

    fixtures: [
        Circle {
            id: bodyCircle
            radius: root.width / 2
            density: 100
            friction: 0.0
            restitution: 0.0
            categories: GameItem.PhysicsCharacter
            collidesWith: GameItem.PhysicsStaticItem | GameItem.PhysicsCharacter

            onBeginContact: {
                if (!isPlayer)
                    return

                var target = other.getBody().target
                if (target.itemType) {
                    //console.log("---------> Begin contact " + other + " | " + target.itemType)
                    target.playerOnItem = true
                }
            }

            onEndContact: {
                if (!isPlayer)
                    return

                var target = other.getBody().target
                if (target.itemType) {
                    //console.log("---------> End contact " + other + " | " + target.itemType)
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
                    //console.log("---------> Begin contact " + other + " | " + target.itemType)
                    target.playerOnItem = true
                }
            }

            onEndContact: {
                if (!isPlayer)
                    return

                var target = other.getBody().target
                if (target.itemType) {
                    //console.log("---------> End contact " + other + " | " + target.itemType)
                    target.playerOnItem = false
                }
            }

        },
        Polygon {
            id: auraSensor
            sensor: true
            categories: GameItem.PhysicsSensor
            collidesWith: GameItem.PhysicsAll
            vertices: [
                Qt.point(root.width / 2, root.height / 2),
                Qt.point(root.width / 2 + auraRadius * Math.cos(character.angle + Math.PI / 4), root.width / 2 + auraRadius * Math.sin(character.angle + Math.PI / 4)),
                Qt.point(root.width / 2 + auraRadius * Math.cos(character.angle + Math.PI / 8), root.width / 2 + auraRadius * Math.sin(character.angle + Math.PI / 8)),
                Qt.point(root.width / 2 + auraRadius * Math.cos(character.angle), root.width / 2 + auraRadius * Math.sin(character.angle)),
                Qt.point(root.width / 2 + auraRadius * Math.cos(character.angle - Math.PI / 8), root.width / 2 + auraRadius * Math.sin(character.angle - Math.PI / 8)),
                Qt.point(root.width / 2 + auraRadius * Math.cos(character.angle - Math.PI / 4), root.width / 2 + auraRadius * Math.sin(character.angle - Math.PI / 4))
            ]

            onBeginContact: {
                if (!isPlayer)
                    return

                var target = other.getBody().target
                if (target.itemType) {
                    //console.log("---------> Begin contact " + other + " | " + target.itemType)
                    target.playerAuraRange = true
                }
            }

            onEndContact: {
                if (!isPlayer)
                    return

                var target = other.getBody().target
                if (target.itemType) {
                    //console.log("---------> End contact " + other + " | " + target.itemType)
                    target.playerAuraRange = false
                }
            }
        },
        Polygon {
            id: hitAttackSensor
            sensor: true
            categories: hitAttackTimer.running ? GameItem.PhysicsAll : GameItem.PhysicsNone
            collidesWith: GameItem.PhysicsAll

            vertices: [
                Qt.point(root.width / 2, root.height / 2),
                Qt.point(root.width / 2 + hitAttackRadius * Math.cos(character.angle + Math.PI / 8), root.width / 2 + hitAttackRadius * Math.sin(character.angle + Math.PI / 8)),
                Qt.point(root.width / 2 + hitAttackRadius * Math.cos(character.angle), root.width / 2 + hitAttackRadius * Math.sin(character.angle)),
                Qt.point(root.width / 2 + hitAttackRadius * Math.cos(character.angle - Math.PI / 8), root.width / 2 + hitAttackRadius * Math.sin(character.angle - Math.PI / 8)),
            ]

            onBeginContact: {
                var target = other.getBody().target
                if (target.itemType && target.enemy) {
                    //console.log("---------> Attack begin contact " + other + " | " + target.enemy)
                    hitAttackTimer.restart()
                    Game.world.performHitAttack(root.character, target.enemy)
                    //target.body.applyLinearImpulse(Qt.point(target.body.getMass() * 0.01, target.body.getMass() * 0), target.body.getWorldCenter())
                }
            }
        }
    ]


    Timer {
        id: hitAttackTimer
        interval: 400
        repeat: false
        onTriggered: root.hitAttackRadius = character.size.width / 3 * app.gridSize
    }

    Connections {
        id: attackConnections
        target: root.character
        onHit: {
            if (hitAttackTimer.running)
                return

            console.log("Hit !!!!!")
            root.hitAttackRadius = character.size.width / 2 * app.gridSize + (2 * app.gridSize)
            hitAttackTimer.restart()
        }
    }

    ItemDescription {
        id: nameLabel
        anchors.bottom: root.top
        anchors.horizontalCenter: root.horizontalCenter
        text: character ? character.name : ""
        opacity: character ? (Game.debugging ? 0.5 : (root.character.playerFocus && !root.isPlayer ? 1 : 0)) : 0
    }

    Image {
        id: playerImage
        anchors.fill: parent
        source: character.heading === Character.HeadingRight ? dataDirectory + "/images/characters/player-male.png" : dataDirectory + "/images/characters/player-male-mirror.png"
        opacity: Game.debugging ? 0.5 : 1
    }

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
            healthIndicator.opacity = 1
            healthIndicatorTimer.restart()
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

    RowLayout {
        id: healthIndicator
        height: app.gridSize / 6
        width: app.gridSize * 2
        anchors.horizontalCenter: root.horizontalCenter
        anchors.bottom: nameLabel.top
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

    SequentialAnimation {
        id: damageAnimation

        ParallelAnimation {
            NumberAnimation {
                target: damageIndicator
                properties: "opacity"
                duration:  300
                from: 0
                to: 0.6
            }

            ScaleAnimator {
                target: damageIndicator
                duration:  300
                from: 1
                to: 1.5
            }
        }

        NumberAnimation {
            target: damageIndicator
            properties: "opacity"
            duration:  100
            to: 0
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
//        RadialGradient {
//            anchors.fill: parent
//            gradient: Gradient {
//                GradientStop { position: 0.0; color: "transparent" }
//                GradientStop { position: 0.5; color: "red" }
//            }
//        }

    }

}
