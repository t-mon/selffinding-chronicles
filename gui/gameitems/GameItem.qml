import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0

import Chronicles 1.0

import "../components"

Item {
    id: root

    property QtObject gameItem: null

    Component.onCompleted: console.log("Created game item " + gameItem.name + " " + gameItem.imageName)

    Image {
        id: itemImage
        anchors.fill: parent
        source: dataDirectory + gameItem.imageName
        opacity: Game.debugging ? 0.5 : 1
    }

    Rectangle {
        id: wireFrame
        anchors.fill: parent
        color: "gray"
        border.color: "black"
        opacity: 0.1
        border.width: 2
        visible: gameItem.playerFocus || Game.debugging
        radius: gameItem.shape === GameObject.ShapeCircle ? width / 2 : 0
    }

    GameLabel {
        id: nameLabel
        anchors.bottom: root.top
        anchors.horizontalCenter: root.horizontalCenter
        text: gameItem.name
        color: "black"
        visible: gameItem.playerFocus || Game.debugging
    }
}

