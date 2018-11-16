import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0

import Chronicles 1.0

import "../components"

Item {
    id: root

    property ChestItem chestItem: Game.world.currentChestItem

    Rectangle {
        anchors.fill: parent
        color: "black"
        opacity: 0.4
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: app.margins
        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true

            ColumnLayout {
                anchors.fill: parent

                GameLabel {
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignHCenter
                    horizontalAlignment: Text.AlignHCenter
                    text: qsTr("Plunder, harr harr")
                    font.pixelSize: app.largeFont
                    color: "white"
                }
            }
        }
    }
}
