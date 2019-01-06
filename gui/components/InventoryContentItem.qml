import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.0

import Chronicles 1.0

import "../components"

Item {
    id: root

    property GameItem selectedItem

    ColumnLayout {
        anchors.fill: parent
        spacing: app.margins / 2

        Rectangle {
            id: tabsRectangle
            Layout.fillWidth: true
            Layout.preferredHeight: app.menuItemSize
            color: "#55000000"
            border.color: "white"
            border.width: app.borderWidth

            ListModel {
                id: inventoryHeaderModel
                ListElement {
                    name: qsTr("Weapons")
                    image: "/images/items/weapons/sword.png"
                    type: GameItem.TypeWeapon
                }
                ListElement {
                    name: qsTr("Firearms")
                    image: "/images/items/firearms/hunting-bow.png"
                    type: GameItem.TypeFirearm
                }
                ListElement {
                    name: qsTr("Plants")
                    image: "/images/items/plants/healing-herb.png"
                    type: GameItem.TypePlant
                }
            }

            ListView {
                id: inventoryHeaderListView
                anchors.fill: parent
                anchors.margins: app.margins / 2
                orientation: Qt.Horizontal
                model: inventoryHeaderModel
                spacing: 0
                clip: true
                flickableDirection: Flickable.AutoFlickIfNeeded
                delegate: InventoryTabButton {
                    selected: ListView.isCurrentItem
                    width: inventoryHeaderListView.width / inventoryHeaderModel.count
                    height: inventoryHeaderListView.height
                    gameItemType: type
                    imageName: image
                    onSelectedChanged: {
                        if (selected) {
                            inventoryGridView.model.itemTypeFilter = type
                            if (inventoryItemModel.count === 0) {
                                root.selectedItem = null
                            } else {
                                root.selectedItem = inventoryItemModel.get(inventoryGridView.currentIndex)
                            }

                        }
                    }

                    onClicked: inventoryHeaderListView.currentIndex = index
                }
            }

        }

        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "#55000000"
            border.color: "white"
            border.width: app.borderWidth

            GridView {
                id: inventoryGridView
                anchors.fill: parent
                anchors.margins: app.margins / 2

                cellWidth: width / 5
                cellHeight: cellWidth
                clip: true
                focus: true

                model: GameItemsProxy {
                    id: inventoryItemModel
                    gameItems: Game.world.player.inventory
                    filterDuplicates: true
                }

                delegate: ContentItem {
                    id: inventoryContetItem
                    selected: GridView.isCurrentItem
                    item: inventoryItemModel.get(index)
                    width: inventoryGridView.cellWidth
                    height: inventoryGridView.cellHeight
                    itemCount: countModel.count

                    GameItemsProxy {
                        id: countModel
                        gameItems: Game.world.player.inventory
                        itemIdFilter: inventoryContetItem.item ? inventoryContetItem.item.itemId : ""
                    }

                    onSelectedChanged: {
                        root.selectedItem = inventoryGridView.model.get(inventoryGridView.currentIndex)
                    }
                }
            }

            MouseArea {
                anchors.fill: inventoryGridView
                onClicked: {
                    var clickedIndex = inventoryGridView.indexAt(mouseX, mouseY)
                    console.log("Clicked on --> " + clickedIndex)
                    if (clickedIndex >= 0)
                        inventoryGridView.currentIndex = clickedIndex
                }
            }
        }
    }
}


