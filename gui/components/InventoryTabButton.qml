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
        anchors.fill: parent
        anchors.margins: app.margins / 2
        border.color: root.selected ? "white" : "gray"
        border.width: app.borderWidth
        color: root.selected ? "green" : "gray"

        Item {
            anchors.centerIn: parent
            height: parent.height
            width: height

            Image {
                id: tabImage
                anchors.fill: parent
                anchors.margins: app.margins / 2
                source: dataDirectory + root.imageName
            }
        }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: root.clicked()
    }

}
