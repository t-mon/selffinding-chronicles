import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.0

import Chronicles 1.0

import "../components"

Item {
    id: root

    property GameItemsProxy itemsProxy

    Rectangle {
        anchors.fill: parent
        color: "transparent"
        border.color: "white"
        border.width: app.borderWidth

        GridView {
            id: gridView
            anchors.fill: parent
            anchors.margins: app.margins
            cellWidth: width / 6
            cellHeight: cellWidth
            clip: true
            model: root.itemsProxy

            delegate: ContentItem {
                selected: GridView.isCurrentItem
                item: itemsProxy.get(index)
                width: gridView.cellWidth
                height: gridView.cellHeight
            }
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                //var clickedItem = gridView.itemAt(mouseX, mouseY)
                var clickedIndex = gridView.indexAt(mouseX, mouseY)
                console.log("Clicked on --> " + clickedIndex)

                if (clickedIndex >= 0)
                    gridView.currentIndex = clickedIndex

            }
        }
    }
}
