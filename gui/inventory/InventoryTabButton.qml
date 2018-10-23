import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.0

import Chronicles 1.0

import "../components"

Item {
    id: root

    property string imageName
    property int gameItemType

    property bool selected: Game.world.player.inventoryProxy.itemTypeFilter === root.gameItemType

    Rectangle {
        anchors.fill: parent
        anchors.margins: app.margins / 2
        //color: "#aaFFFFFF"
        //border.width: app.borderWidth
        //radius: root.height / 8

        LinearGradient {
            anchors.fill: parent
            start: Qt.point(0, 0)
            end: Qt.point(0, parent.height)
            gradient: Gradient {
                GradientStop { position: 0.0; color: "white" }
                GradientStop { position: 1.0; color: root.selected ? "green" : "gray" }
            }
        }

        Image {
            id: tabImage
            anchors.centerIn: parent
            height: parent.height - app.margins
            width: height
            source: dataDirectory + root.imageName
        }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: Game.world.player.inventoryProxy.itemTypeFilter = root.gameItemType
    }
}
