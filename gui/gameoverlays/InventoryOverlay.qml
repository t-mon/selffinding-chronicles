import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0

import Chronicles 1.0

import "../components"

GameOverlayItem {
    id: root

    RowLayout {
        id: inventoryLayout
        anchors.fill: parent
        anchors.margins: app.margins
        spacing: app.margins / 2

        Item {
            id: informationItem
            Layout.fillHeight: true
            Layout.preferredWidth: app.menuItemSize * 5 + app.margins

            ColumnLayout {
                id: informationColumnLayout
                anchors.fill: parent
                spacing: app.margins / 2

                CharacterDescription {
                    id: characterDescription
                    Layout.fillWidth: true
                    Layout.preferredHeight: app.gridSize * 9
                    character: Game.engine.player
                }

                ContentItemDescription {
                    id: itemDescription
                    Layout.fillWidth: true
                    Layout.minimumHeight: app.gridSize * 7
                    Layout.fillHeight: true
                    item: null
                }
            }
        }

        Item {
            id: spacingItem
            Layout.fillWidth: true
            Layout.fillHeight: true

            EquipmentContentItem {
                id: equipmentRow
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: parent.bottom
                //anchors.bottomMargin: app.margins
            }
        }

        Item {
            id: inventoryItem
            Layout.fillHeight: true
            Layout.preferredWidth: app.menuItemSize * 5 + app.margins

            ColumnLayout {
                id: inventoryColumnLayout
                anchors.fill: parent
                spacing: app.margins / 2

                InventoryContentItem {
                    id: inventoryContentItem
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    onSelectedItemChanged: {
                        if (!selectedItem) {
                            itemDescription.item = null
                            return
                        }

                        console.log("Selected item " + selectedItem.name)
                        itemDescription.item = selectedItem
                    }
                }

                RowLayout {
                    id: actionRow
                    Layout.fillWidth: true
                    spacing: app.margins / 2

                    GameButton {
                        Layout.fillWidth: true
                        enabled: inventoryContentItem.selectedItem
                        text: {
                            if (!inventoryContentItem.selectedItem)
                                return qsTr("Unknown");

                            return inventoryContentItem.selectedItem.interactionString
                        }
                        onClicked: {
                            console.log("Use clicked for", inventoryContentItem.selectedItem, inventoryContentItem.selectedItem.itemTypeName)
                            inventoryContentItem.useCurrentItem()
                        }
                    }

                    GameButton {
                        Layout.fillWidth: true
                        text: qsTr("Drop")
                        enabled: false
                        onClicked: console.log("Drop clicked")
                    }

                    GameButton {
                        Layout.fillWidth: true
                        visible: Game.engine.playerController.controlMode === PlayerController.ControlModeTouchscreen
                        text: "Close"
                        onClicked: Game.engine.inventoryClicked()
                    }
                }
            }
        }
    }
}
