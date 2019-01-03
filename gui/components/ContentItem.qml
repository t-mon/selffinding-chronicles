import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.0

import Chronicles 1.0

import "../components"

Item {
    id: root

    property GameItem item
    property bool selected: false
    property int itemCount: 0

    onItemCountChanged: console.log("Item count changed for", item.name, itemCount)

    signal clicked()

    Rectangle {
        id: itemRectangle
        anchors.fill: parent
        anchors.margins: app.margins / 2
        border.color: "white"
        border.width: app.borderWidth
        color: root.selected ? "brown" : "gray"

        Image {
            id: itemImage
            anchors.fill: parent
            anchors.margins: parent.width / 8
            source: root.item ? dataDirectory + root.item.imageName : ""
        }
    }

    Label {
        anchors.centerIn: parent
        text: root.itemCount
        color: "white"
    }

    MouseArea {
        id: itemMouseArea
        anchors.fill: parent
        preventStealing: true
        onClicked: {
            console.log("Clicked on " + item.name)
            root.clicked()
        }
    }
}


