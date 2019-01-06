import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.0

import Chronicles 1.0

Item {
    id: root

    property GameItems gameItems
    property GameItem selectedGameItem

    //onSelectedGameItemChanged: console.log("Selected item:", (selectedGameItem ? selectedGameItem.name : "no item left"))

    property real itemSize: app.gridSize * 2.5

    Rectangle {
        anchors.fill: parent
        color: "#55000000"
        border.color: "white"
        border.width: app.borderWidth

        GridView {
            id: itemsGridView
            anchors.fill: parent
            anchors.margins: app.margins
            cellWidth: width / 5
            cellHeight: cellWidth
            clip: true
            focus: true

            model: GameItemsProxy {
                id: itemModel
                gameItems: root.gameItems
                filterDuplicates: true
            }

            delegate: ContentItem {
                id: contetItem
                selected: GridView.isCurrentItem
                item: itemModel.get(index)
                width: itemsGridView.cellWidth
                height: itemsGridView.cellHeight
                itemCount: countModel.count
                onItemCountChanged: {
                    if (itemCount === 0) {
                        selectedGameItem = null
                    }
                }

                onSelectedChanged: {
                    if (selected) {
                        root.selectedGameItem = itemModel.get(index)
                    }
                }

                GameItemsProxy {
                    id: countModel
                    gameItems: root.gameItems
                    itemIdFilter: item ? item.itemId : ""
                }
            }
        }

        MouseArea {
            anchors.fill: itemsGridView
            onClicked: {
                var clickedIndex = itemsGridView.indexAt(mouseX, mouseY)
                console.log("Clicked on --> " + clickedIndex)

                if (clickedIndex >= 0)
                    itemsGridView.currentIndex = clickedIndex

            }
        }
    }
}
