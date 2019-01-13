import QtQuick 2.12
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

    //onItemCountChanged: console.log("Item count changed for", (item ? item.name : ""), itemCount)

    signal clicked()

    Rectangle {
        id: itemRectangle
        anchors.fill: parent
        anchors.margins: app.margins / 2
        border.color: root.selected ? "white" : "gray"
        border.width: app.borderWidth
        color: root.selected ? "green" : "gray"
        opacity: 0.7
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
            console.log("Clicked on " + item.name)
            root.clicked()
        }
    }
}


