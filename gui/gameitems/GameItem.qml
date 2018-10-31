import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0

import Chronicles 1.0

import "../components"

Item {
    id: root

    property GameItem gameItem: Game.world.gameItems.get(model.index)
    antialiasing: app.antialiasing

    opacity: gameItem ? (gameItem.hidingPlayer ? 0.5 : 1) : 0

    //    Component.onCompleted: console.log("Created game item " + gameItem.name + " " + gameItem.imageName)
    //    Component.onDestruction: console.log("Destroy item")

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

    Rectangle {
        id: wireFrame
        anchors.fill: parent
        color: "gray"
        border.color: "black"
        opacity: 0.1
        border.width: 2
        visible: gameItem ? gameItem.playerFocus || Game.debugging : Game.debugging
        radius: gameItem ? (gameItem.shape === GameObject.ShapeCircle ? width / 2 : 0) : 0
    }


    ItemDescription {
        id: nameLabel
        anchors.bottom: root.top
        anchors.horizontalCenter: root.horizontalCenter
        text: gameItem ? gameItem.name : ""
        opacity: gameItem ? (Game.debugging ? 0.5 : (gameItem.playerFocus ? 1 : 0)) : 0
    }

}

