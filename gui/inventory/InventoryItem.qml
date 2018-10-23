import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.0

import Chronicles 1.0

import "../components"

Item {
    id: root

    property real cellSize

    ColumnLayout {
        id: inventoryColumnLayout
        anchors.fill: parent
        spacing: app.margins

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

                        property bool selected: false

                        Rectangle {
                            id: itemRectangle
                            anchors.fill: parent
                            anchors.margins: app.margins / 2
                            //color: "#AAFFFFFF"
                            //radius: width / 8
                            border.color: "white"
                            border.width: app.borderWidth

                            LinearGradient {
                                anchors.fill: parent
                                start: Qt.point(0, 0)
                                end: Qt.point(0, parent.height)
                                gradient: Gradient {
                                    GradientStop { position: 0.0; color: "white" }
                                    GradientStop { position: 1.0; color: root.selected ? "green" : "gray" }
                                }
                            }

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

