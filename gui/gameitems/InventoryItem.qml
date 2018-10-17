import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0

import Chronicles 1.0

import "../components"

Item {
    id: root

    property real cellSize: width * 0.1
    property real borderWidth: cellSize / 30

    Rectangle {
        id: backgroundRectangle
        anchors.fill: parent
        color: "black"
        opacity: 0.9
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: app.margins
        spacing: app.margins

        RowLayout {
            spacing: app.margins

            Rectangle {
                Layout.fillWidth: true
                Layout.minimumHeight: cellSize
                color: "transparent"
                border.color: "white"
                border.width: borderWidth
            }
        }

        RowLayout {
            spacing: app.margins

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
                                border.width: borderWidth
                                radius: width / 8

                                Image {
                                    id: itemImage
                                    anchors.fill: parent
                                    anchors.margins: itemRectangle.width / 8
                                    source: dataDirectory + model.imageName
                                    opacity: Game.debugging ? 0.5 : 1
                                }
                            }
                        }
                    }
                }
            }

            Item {
                Layout.minimumWidth: cellSize * 4
                Layout.fillHeight: true

                Rectangle {
                    anchors.fill: parent
                    color: "transparent"
                    border.color: "white"
                    border.width: borderWidth
                }

                ColumnLayout {
                    id: desciptionLayout
                    anchors.fill: parent
                    anchors.margins: app.margins

                    Label {
                        Layout.fillWidth: true
                        font.bold: true
                        font.family: app.fontFamily
                        font.pixelSize: app.mediumFont
                        font.weight: Font.ExtraBold
                        z: Map.Layer4Highest
                        color: "white"
                        text: Game.world.player.name
                    }

                    ColumnLayout {
                        id: playerPropertyLayout
                        Layout.fillWidth: true
                        //Layout.fillHeight: true
                        spacing: 0

                        Label {
                            font.bold: true
                            font.family: app.fontFamily
                            font.pixelSize: app.mediumFont
                            font.weight: Font.ExtraBold
                            z: Map.Layer4Highest
                            color: "white"
                            text: "Experience: " + Game.world.player.experience
                        }

                        Label {
                            font.bold: true
                            font.family: app.fontFamily
                            font.pixelSize: app.mediumFont
                            font.weight: Font.ExtraBold
                            z: Map.Layer4Highest
                            color: "white"
                            text: "Health: " + Game.world.player.health
                        }

                        Label {
                            font.bold: true
                            font.family: app.fontFamily
                            font.pixelSize: app.mediumFont
                            font.weight: Font.ExtraBold
                            z: Map.Layer4Highest
                            color: "white"
                            text: "Mana: " + Game.world.player.mana
                        }

                        Label {
                            font.bold: true
                            font.family: app.fontFamily
                            font.pixelSize: app.mediumFont
                            font.weight: Font.ExtraBold
                            z: Map.Layer4Highest
                            color: "white"
                            text: "Wisdom: " + Game.world.player.wisdom
                        }

                        Label {
                            font.bold: true
                            font.family: app.fontFamily
                            font.pixelSize: app.mediumFont
                            font.weight: Font.ExtraBold
                            z: Map.Layer4Highest
                            color: "white"
                            text: "Strength: " + Game.world.player.strength
                        }

                        Label {
                            font.bold: true
                            font.family: app.fontFamily
                            font.pixelSize: app.mediumFont
                            font.weight: Font.ExtraBold
                            z: Map.Layer4Highest
                            color: "white"
                            text: "Stealth: " + Game.world.player.stealth
                        }
                    }
                }
            }
        }

        RowLayout {
            spacing: app.margins

            Rectangle {
                Layout.fillWidth: true
                Layout.minimumHeight: cellSize
                color: "transparent"
                border.color: "white"
                border.width: borderWidth
            }
        }
    }
}
