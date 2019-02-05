import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0

import Chronicles 1.0

import "../components"

Item {
    id: root

    property GameItem item
    property bool selected: false
    property int itemCount: 0

    property bool equipped: (Game.engine.player && Game.engine.player.weapon && item ? item.itemId === Game.engine.player.weapon.itemId : false) ||
                            (Game.engine.player &&Game.engine.player.firearm && item ? item.itemId === Game.engine.player.firearm.itemId : false)

    //onItemCountChanged: console.log("Item count changed for", (item ? item.name : ""), itemCount)

    signal clicked()

    Rectangle {
        id: itemRectangle
        anchors.fill: parent
        anchors.margins: app.margins / 2
        color: root.selected ? "steelblue" : "gray"
        border.color: root.equipped ? "red" : "white"
        border.width: app.borderWidth
        radius: app.margins / 2
        opacity: 0.4
    }

    Image {
        id: itemImage
        anchors.fill: parent
        anchors.margins: app.margins
        source: root.item ? dataDirectory + root.item.imageName : ""
    }

    GameLabel {
        anchors.right: parent.right
        anchors.rightMargin: app.margins
        anchors.bottom: parent.bottom
        anchors.bottomMargin: app.margins / 2
        text: root.itemCount
        color: "white"
        visible: root.itemCount > 1
    }

    MouseArea {
        id: itemMouseArea
        anchors.fill: parent
        preventStealing: true
        onClicked: {
            if (item)
                console.log("Clicked on " + item.name)

            root.clicked()
        }
    }
}


