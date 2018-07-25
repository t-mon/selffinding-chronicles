import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0

import Chronicles 1.0

import "../components"

Item {
    id: root

    property var field: null
    property real cellSize: 1

//    // TODO: load different items depending on the type
//    width: field.gameItem.size.width * root.cellSize
//    height: field.gameItem.size.height * root.cellSize

    Component.onCompleted: console.log("-------->" + width + "  " + height)

    Image {
        id: itemImage
        source: field.gameItem.imageName
        anchors.fill: parent
        z: 1
    }

    GameLabel {
        id: nameLabel
        anchors.bottom: itemImage.top
        anchors.horizontalCenter: itemImage.horizontalCenter
        text: field.gameItem.name
        color: "black"
        visible: field.inPlayerRange
        z: 2
    }
}

