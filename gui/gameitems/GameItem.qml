import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0

import Chronicles 1.0

import "../components"

Item {
    id: root

    property var gameItem: null

    Component.onCompleted: console.log("Created game item " + gameItem.name + " " + gameItem.imageName)
    Component.onDestruction: console.log("Destroy item")

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

    GameLabel {
        id: nameLabel
        anchors.bottom: root.top
        anchors.horizontalCenter: root.horizontalCenter
        text: gameItem ? gameItem.name : ""
        color: "black"
        visible: gameItem ? gameItem.playerFocus || Game.debugging : Game.debugging
    }

}

