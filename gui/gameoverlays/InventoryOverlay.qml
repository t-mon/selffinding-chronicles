import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0

import Chronicles 1.0

import "../components"

GameOverlayItem {
    id: root

    RowLayout {
        id: inventoryLoyout
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
                                return qsTr("Use");

                            switch (inventoryContentItem.selectedItem.itemType) {
                            case GameItem.TypeWeapon:
                            case GameItem.TypeFirearm:
                                return qsTr("Arm");
                            default:
                                return qsTr("Use");
                            }
                        }
                        onClicked: {
                            console.log("Use clicked for", inventoryContentItem.selectedItem, inventoryContentItem.selectedItem.itemTypeName)
                            switch (inventoryContentItem.selectedItem.itemType) {
                            case GameItem.TypeWeapon:
                                Game.engine.player.weapon = inventoryContentItem.selectedItem
                                break;
                            case GameItem.TypeFirearm:
                                Game.engine.player.firearm = inventoryContentItem.selectedItem
                                break;
                            default:
                                console.log("Not implemented yet for item", inventoryContentItem.selectedItem.itemTypeName)
                                break;
                            }
                        }
                    }

                    GameButton {
                        Layout.fillWidth: true
                        text: "Drop"
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
