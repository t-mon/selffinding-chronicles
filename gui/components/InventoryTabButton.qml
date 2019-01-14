import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.0

import Chronicles 1.0

import "../components"

Item {
    id: root

    property string imageName: ""
    property int gameItemType: GameItem.TypeNone
    property bool selected: false

    signal clicked()

    Rectangle {
        id: backgroundRectanlge
        anchors.fill: parent
        anchors.margins: app.margins / 2
        color: root.selected ? "steelblue" : "gray"
        border.color: "white"
        border.width: app.borderWidth
        radius: app.margins / 2
        opacity: 0.4
    }

    Item {
        anchors.centerIn: backgroundRectanlge
        height: backgroundRectanlge.height
        width: height

        Image {
            id: tabImage
            anchors.fill: parent
            anchors.margins: app.margins / 2
            source: dataDirectory + root.imageName
        }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: root.clicked()
    }

}
