import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0

import Chronicles 1.0

import "../components"

GameOverlayItem {
    id: root

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
                Layout.preferredWidth: app.menuItemSize * 5 + app.margins
                Layout.fillHeight: true

                ColumnLayout {
                    id: plunderInventoryItem
                    anchors.fill: parent
                    spacing: app.margins / 2

                    ObjectContentItem {
                        id: plunderInventory
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        gameItems: Game.world.currentPlunderItems
                        onSelectedGameItemChanged: {
                            if (!selectedGameItem) {
                                itemDescription.item = null
                                return
                            }

                            console.log("Selected item " + selectedGameItem.name)
                            itemDescription.item = selectedGameItem
                        }
                    }


                    ContentItemDescription {
                        id: itemDescription
                        Layout.fillWidth: true
                        Layout.preferredHeight: app.gridSize * 10
                        item: null
                    }

                    RowLayout {
                        id: plunderOptionsRow
                        Layout.fillWidth: true
                        spacing: app.margins / 2

                        GameButton {
                            Layout.fillWidth: true
                            text: qsTr("Take all")
                            onClicked: {
                                Game.world.takeAllItems(Game.world.currentPlunderItems)
                                Game.world.finishPlunder()
                            }
                        }

                        GameButton {
                            Layout.fillWidth: true
                            text: qsTr("Take")
                            onClicked: {
                                if (plunderInventory.selectedGameItem) {
                                    console.log("Take item", plunderInventory.selectedGameItem.name)
                                    Game.world.takeItem(Game.world.currentPlunderItems, plunderInventory.selectedGameItem)
                                }
                            }
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
                id: spacingItem
                Layout.fillWidth: true
                Layout.fillHeight: true
            }

            Item {
                id: playerInventoryItem
                Layout.fillHeight: true
                Layout.preferredWidth: app.menuItemSize * 5 + app.margins

                InventoryContentItem {
                    id: playerInventory
                    anchors.fill: parent
                }
            }
        }
    }
}
