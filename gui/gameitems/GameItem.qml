import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0

import Box2D 2.0
import Chronicles 1.0

import "../components"
import "../physics"

PhysicsItem {
    id: root

    property GameItem gameItem: Game.world.gameItems.get(model.index)
    property int itemType: gameItem ? gameItem.itemType : GameItem.TypeNone

    antialiasing: app.antialiasing
    opacity: gameItem ? (gameItem.hidingPlayer ? 0.5 : 1) : 0
    onPlayerAuraRangeChanged: gameItem.playerVisible = playerAuraRange

    bodyType: Body.Dynamic

    fixtures: [
        Circle {
            id: itemBody
            radius: root.width / 2
            density: 100
            friction: 0
            restitution: 0.0
            categories: {
                if (!gameItem)
                    return Box.Category2

                if (gameItem.itemType == gameItem.TypeChest) {
                    return Box.All
                } else {
                    return Box.Category2
                }
            }
        },

        Circle {
            id: itemSensor
            radius: root.width / 2
            density: 0
            friction: 0
            restitution: 0
            sensor: true
        }
    ]

    Connections {
        target: root.gameItem
        onPlayerOnItemChanged: {
            console.log("Player on item " + playerOnItem)
            if (root.gameItem.itemType === GameItem.TypeTree && playerOnItem) {
                console.log("Start animation")
                playerOnItemAnimation.start()
            }
        }
    }

    SequentialAnimation {
        id: playerOnItemAnimation
        loops: 1

        RotationAnimation {
            target: root
            property: "rotation"
            to: 3
            duration: 80
        }

        RotationAnimation {
            target: root
            property: "rotation"
            to: 0
            duration: 80
        }

        RotationAnimation {
            target: root
            property: "rotation"
            to: -3
            duration: 80
        }

        RotationAnimation {
            target: root
            property: "rotation"
            to: 0
            duration: 80
        }
    }

    Image {
        id: itemImage
        anchors.fill: parent
        source: gameItem ? dataDirectory + gameItem.imageName : ""
        opacity: Game.debugging ? 0.5 : 1
    }

    ItemDescription {
        id: nameLabel
        anchors.bottom: root.top
        anchors.horizontalCenter: root.horizontalCenter
        text: gameItem ? gameItem.name : ""
        opacity: gameItem ? (Game.debugging ? 0.5 : (gameItem.playerFocus ? 1 : 0)) : 0
    }

    //    Component.onCompleted: console.log("Created game item " + gameItem.name + " " + gameItem.imageName)
    //    Component.onDestruction: console.log("Destroy item")

}

