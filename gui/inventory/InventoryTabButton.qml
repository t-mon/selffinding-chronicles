import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0

import Chronicles 1.0

import "../components"

Item {
    id: root

    property string imageName
    property int gameItemType

    property bool selected: Game.world.player.inventoryProxy.itemTypeFilter === root.gameItemType

    Rectangle {
        anchors.centerIn: parent
        height: parent.height
        width: height
        anchors.margins: app.margins / 2
        color: "#aaFFFFFF"
        border.color: root.selected ? "steelblue" : "gray"
        border.width: app.borderWidth
        radius: width / 8

        Image {
            id: tabImage
            anchors.fill: parent
            anchors.margins: parent.width / 8
            source: dataDirectory + root.imageName
        }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: Game.world.player.inventoryProxy.itemTypeFilter = root.gameItemType
    }
}
