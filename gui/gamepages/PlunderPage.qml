import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0

import Chronicles 1.0

import "../components"

GameOverlayPage {
    id: root

    property real cellSize: width * 0.1

    Rectangle {
        anchors.fill: parent
        color: "black"
        opacity: 0.4
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: app.margins
        spacing: app.margins / 2

        RowLayout {
            id: inventoryLoyout
            Layout.fillHeight: true
            Layout.fillWidth: true
            spacing: app.margins / 2


            Item {
                Layout.fillHeight: true
                Layout.fillWidth: true

                ColumnLayout {
                    id: plunderInventoryItem
                    anchors.fill: parent
                    spacing: app.margins / 2

                    ObjectContentItem {
                        id: plunderInventory
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        itemsProxy: Game.world.currentPlunderItemsProxy
                    }

                    RowLayout {
                        id: plunderOptionsRow
                        Layout.fillWidth: true
                        Layout.preferredHeight: app.gridSize * 3
                        spacing: app.margins / 2

                        GameButton {
                            Layout.fillWidth: true
                            text: qsTr("Take all")
                            onClicked: Game.world.finishPlunder()
                        }

                        GameButton {
                            Layout.fillWidth: true
                            text: qsTr("Close")
                            onClicked: Game.world.finishPlunder()
                        }
                    }
                }
            }

            Item {
                id: playerInventoryItem
                Layout.fillHeight: true
                Layout.fillWidth: true

                InventoryContentItem {
                    id: playerInventory
                    anchors.fill: parent
                }
            }
        }
    }
}
