import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0

import Chronicles 1.0

import "../components"

Item {
    id: root

    property real cellSize: width * 0.1

    Rectangle {
        id: backgroundRectangle
        anchors.fill: parent
        color: "black"
        opacity: 0.9
    }

    RowLayout {
        id: mainRowLoayout
        anchors.fill: parent
        anchors.margins: app.margins
        spacing: app.margins

        InventoryItem {
            Layout.fillHeight: true
            Layout.fillWidth: true
            cellSize: root.cellSize
        }

        ColumnLayout {
            id: informationColumnLayout
            Layout.minimumWidth: cellSize * 4
            Layout.maximumWidth: cellSize * 4
            Layout.fillHeight: true

            Item {
                Layout.fillWidth: true
                //Layout.fillHeight: true

                Rectangle {
                    anchors.fill: parent
                    color: "transparent"
                    border.color: "white"
                    border.width: app.borderWidth
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

                    Column {
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

            Item {
                Layout.fillWidth: true
                Layout.fillHeight: true

                Rectangle {
                    anchors.fill: parent
                    color: "transparent"
                    border.color: "white"
                    border.width: app.borderWidth
                }
            }
        }
    }
}
