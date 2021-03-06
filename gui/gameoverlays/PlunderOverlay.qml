import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0

import Chronicles 1.0

import "../components"

GameOverlayItem {
    id: root

    Item {
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.margins: app.margins
        width: app.gridSize * 15

        ColumnLayout {
            id: plunderInventoryItem
            anchors.fill: parent
            spacing: app.margins / 2

            ObjectContentItem {
                id: plunderInventory
                Layout.fillHeight: true
                Layout.fillWidth: true
                gameItems: Game.engine.currentPlunderItems
                onGameItemsChanged: console.log("Plunder overlay game items changed", Game.engine.currentPlunderItems)
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
                Layout.preferredHeight: app.gridSize * 6
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
                        Game.engine.takeAllItems(Game.engine.currentPlunderItems)
                        Game.engine.finishPlunder()
                    }
                }

                GameButton {
                    Layout.fillWidth: true
                    text: qsTr("Take")
                    onClicked: {
                        if (plunderInventory.selectedGameItem) {
                            console.log("Take item", plunderInventory.selectedGameItem.name)
                            Game.engine.takeItem(Game.engine.currentPlunderItems, plunderInventory.selectedGameItem)
                        }
                    }
                }

                GameButton {
                    Layout.fillWidth: true
                    text: qsTr("Close")
                    onClicked: Game.engine.finishPlunder()
                }
            }
        }
    }
}


//    ColumnLayout {
//        anchors.fill: parent
//        anchors.margins: app.margins
//        spacing: app.margins / 2

//        RowLayout {
//            id: inventoryLayout
//            Layout.fillHeight: true
//            Layout.fillWidth: true
//            spacing: app.margins / 2



//            Item {
//                id: spacingItem
//                Layout.fillWidth: true
//                Layout.fillHeight: true
//            }

//            Item {
//                id: playerInventoryItem
//                Layout.fillHeight: true
//                Layout.preferredWidth: app.menuItemSize * 5 + app.margins

//                InventoryContentItem {
//                    id: playerInventory
//                    anchors.fill: parent
//                }
//            }
//         }
//    }
