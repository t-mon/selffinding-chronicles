import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0

import Box2D 2.0
import Chronicles 1.0

import "../components"
import "../physics"

PhysicsItem {
    id: root

    property Enemy enemy: Game.world.enemyItems.get(model.index)
    property int itemType: enemy ? enemy.itemType : GameItem.TypeNone

    antialiasing: app.antialiasing
    opacity: enemy ? (enemy.hidingPlayer ? 0.5 : 1) : 0
    onPlayerAuraRangeChanged: enemy.playerVisible = playerAuraRange
    onPlayerOnItemChanged: enemy.playerOnItem = playerOnItem

    bodyType: enemy ? enemy.bodyType : GameObject.BodyTypeStatic
    linearDamping: 1
    fixedRotation: true

    fixtures: [
        Circle {
            id: itemBody
            radius: root.width / 2
            density: 1
            friction: 1
            restitution: 0.0
            categories: enemy ? enemy.categoryFlag : GameObject.PhysicsNone
            collidesWith: enemy ? enemy.collisionFlag : GameObject.PhysicsNone
        }
    ]

    Item {
        id: frame
        anchors.fill: parent;
        rotation: -root.rotation

        Rectangle {
            id: frameWire
            color: "transparent";
            border.color: "red";
            border.width: 2;
            opacity: Game.debugging ? 0.5 : 0
        }

        ItemDescription {
            id: nameLabel
            anchors.bottom: frame.top
            anchors.horizontalCenter: frame.horizontalCenter
            text: enemy ? enemy.name : ""
            opacity: enemy ? (Game.debugging ? 0.5 : (enemy.playerFocus ? 1 : 0)) : 0
        }

        Image {
            id: itemImage
            anchors.fill: frame
            source: enemy ? dataDirectory + enemy.imageName : ""
            opacity: Game.debugging ? 0.5 : 1
        }

        RowLayout {
            id: healthIndicator
            height: app.gridSize / 6
            width: app.gridSize * 2
            anchors.horizontalCenter: frame.horizontalCenter
            anchors.bottom: nameLabel.top
            spacing: 0
            opacity: 0

            Behavior on opacity { NumberAnimation { duration: 200 } }

            Rectangle {
                color: "red"
                Layout.fillHeight: true
                Layout.preferredWidth: enemy ? parent.width * enemy.healthPercentage / 100 : 0
            }

            Rectangle {
                color: "gray"
                Layout.fillHeight: true
                Layout.fillWidth: true
            }
        }

    }




    Connections {
        id: healthIndicatorConnections
        target: root.enemy
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


    ParallelAnimation {
        id: damageAnimation

        ScaleAnimator {
            target: damageIndicator
            duration: 400
            from: 1.2
            to: 1.5
        }

        SequentialAnimation {

            NumberAnimation {
                target: damageIndicator
                properties: "opacity"
                duration: 200
                from: 0
                to: 0.5
            }

            NumberAnimation {
                target: damageIndicator
                properties: "opacity"
                duration:  200
                to: 0
            }
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
