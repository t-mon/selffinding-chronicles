import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0

import Chronicles 1.0

Item {
    id: root

    property GameItem item: null

    Rectangle {
        anchors.fill: parent
        color: "transparent"
        border.color: "white"
        border.width: app.borderWidth
    }

    GameLabel {
        id: noSelectionLable
        anchors.centerIn: parent
        visible: !root.item
        font.bold: true
        font.family: app.fontFamily
        font.pixelSize: app.mediumFont
        font.weight: Font.ExtraBold
        color: "white"
        text: "No item selected."
    }

    Column {
        id: playerPropertyLayout
        spacing: app.margins / 2
        anchors.fill: parent
        anchors.margins: app.margins
        visible: root.item

        GameLabel {
            color: "white"
            text: qsTr("Name") + ": " + (root.item ? root.item.name : "")
        }

        GameLabel {
            color: "white"
            text: qsTr("Price") + ": " + (root.item ? root.item.price : "")
        }

    }
}
