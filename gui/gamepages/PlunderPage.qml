import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0

import Chronicles 1.0

import "../components"
import "../inventory"

Item {
    id: root

    property ChestItem chestItem: Game.world.currentChestItem
    property real cellSize: width * 0.1

    Rectangle {
        anchors.fill: parent
        color: "black"
        opacity: 0.4
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: app.margins

        RowLayout {
            id: inventoryLoyout
            Layout.fillHeight: true
            Layout.fillWidth: true
            spacing: app.margins

            Item {
                id: plunderInventoryItem
                Layout.fillHeight: true
                Layout.fillWidth: true

                InventoryItem {
                    id: plunderInventory
                    anchors.fill: parent
                    cellSize: root.cellSize
                }
            }

            Item {
                id: playerInventoryItem
                Layout.fillHeight: true
                Layout.fillWidth: true

                InventoryItem {
                    id: playerInventory
                    anchors.fill: parent
                    cellSize: root.cellSize
                }
            }
        }

        //        Item {
        //            Layout.fillWidth: true
        //            Layout.fillHeight: true

        //            ColumnLayout {
        //                anchors.fill: parent

        //                GameLabel {
        //                    Layout.fillWidth: true
        //                    Layout.alignment: Qt.AlignHCenter
        //                    horizontalAlignment: Text.AlignHCenter
        //                    text: qsTr("Plunder, harr harr")
        //                    font.pixelSize: app.largeFont
        //                    color: "white"
        //                }
        //            }
        //        }

        Item {
            Layout.fillWidth: true
            Layout.preferredHeight: app.gridSize * 3

            RowLayout {
                anchors.left: parent.left
                anchors.right: parent.right
                GameButton {
                    Layout.fillWidth: true
                    text: qsTr("Finish")
                    onClicked: { Game.world.finishPlunder() }
                }
            }

            //Rectangle {anchors.fill: parent; color: "blue" }
        }
    }
}
