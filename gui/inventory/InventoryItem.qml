import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0

import Chronicles 1.0

import "../components"

Item {
    id: root

    property real cellSize

    ColumnLayout {
        id: inventoryColumnLayout
        anchors.fill: parent

        Rectangle {
            Layout.fillWidth: true
            Layout.minimumHeight: cellSize

            color: "transparent"
            border.color: "white"
            border.width: app.borderWidth

            RowLayout {
                anchors.fill: parent
                anchors.margins: app.margins

                InventoryTabButton {
                    id: weaponsTabButton
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    gameItemType: GameItem.TypeWeapon
                    imageName: "/images/items/weapons/sword.png"
                }

                InventoryTabButton {
                    id: plantsTabButton
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    gameItemType: GameItem.TypePlant
                    imageName: "/images/items/plants/healing-herb.png"
                }
            }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "transparent"
            border.color: "white"
            border.width: borderWidth

            GridView {
                id: inventoryGridView
                anchors.fill: parent
                anchors.margins: app.margins

                clip: true
                model: Game.world.player.inventoryProxy
                delegate: inventoryItemDelegate

                cellWidth: width / 6
                cellHeight: cellWidth

                Component {
                    id: inventoryItemDelegate
                    Item {
                        width: inventoryGridView.cellWidth
                        height: inventoryGridView.cellHeight

                        Rectangle {
                            id: itemRectangle
                            anchors.fill: parent
                            anchors.margins: app.margins / 2
                            color: "#aaFFFFFF"
                            border.color: "white"
                            border.width: app.borderWidth
                            radius: width / 8

                            Image {
                                id: itemImage
                                anchors.fill: parent
                                anchors.margins: parent.width / 8
                                source: dataDirectory + model.imageName
                            }
                        }
                    }
                }
            }
        }
    }
}

