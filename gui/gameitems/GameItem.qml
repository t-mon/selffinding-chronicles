import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0

import Chronicles 1.0

import "../components"

Item {
    id: root

    property QtObject gameItem: null

    Image {
        id: itemImage
        source: dataDirectory + gameItem.imageName
        anchors.fill: parent
        opacity: Game.debugging ? 0.5 : 1
    }

    Rectangle {
        id: wireFrame
        anchors.fill: parent
        color: "gray"
        border.color: "black"
        opacity: 0.1
        border.width: 2
        visible: Game.debugging
        radius: gameItem.shape === GameObject.ShapeCircle ? width / 2 : 0
    }

    GameLabel {
        id: nameLabel
        anchors.bottom: itemImage.top
        anchors.horizontalCenter: itemImage.horizontalCenter
        text: gameItem.name
        color: "black"
        visible: Game.debugging
    }
}

