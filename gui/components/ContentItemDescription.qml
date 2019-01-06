import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0

import Chronicles 1.0

Item {
    id: root

    property GameItem item: null

    Rectangle {
        anchors.fill: parent
        color: "#55000000"
        border.color: "white"
        border.width: app.borderWidth
    }

    ColumnLayout {
        id: itemPropertyLayout
        spacing: app.margins / 2
        anchors.fill: parent
        anchors.margins: app.margins
        visible: root.item

        RowLayout {
            Layout.fillWidth: true

            Item {
                Layout.preferredWidth: app.menuItemSize
                Layout.preferredHeight: app.menuItemSize

                Rectangle {
                    id: itemRectangle
                    anchors.fill: parent
                    anchors.margins: app.margins / 2
                    color: "gray"
                    border.color: "white"
                    border.width: app.borderWidth

                    Image {
                        id: itemImage
                        anchors.fill: parent
                        anchors.margins: parent.width / 8
                        source: root.item ? dataDirectory + root.item.imageName : ""
                    }
                }
            }

            GameLabel {
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignVCenter
                text: (root.item ? root.item.name : "")
                font.pixelSize: largeFont
                color: "white"
            }
        }



        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: app.borderWidth / 2
            color: "white"
        }

        GameLabel {
            color: "white"
            text: qsTr("Price") + ": " + (root.item ? root.item.price : "")
        }

        Item {
            id: spacingItem
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }
}
