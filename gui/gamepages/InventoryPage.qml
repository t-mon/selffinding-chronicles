import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0

import Chronicles 1.0

import "../components"

GameOverlayPage {
    id: root

    Rectangle {
        id: backgroundRectangle
        anchors.fill: parent
        color: "black"
        opacity: 0.7
    }

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
                    character: Game.world.player
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
                        text: "Use"
                        onClicked: console.log("Use clicked")
                    }

                    GameButton {
                        Layout.fillWidth: true
                        text: "Drop"
                        onClicked: console.log("Drop clicked")
                    }
                }
            }
        }
    }
}
