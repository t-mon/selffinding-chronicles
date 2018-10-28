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
        opacity: 0.7
    }

    RowLayout {
        id: inventoryLoyout
        anchors.fill: parent
        anchors.margins: app.margins
        spacing: app.margins

        Item {
            id: informationItem
            Layout.fillHeight: true
            Layout.preferredWidth: root.cellSize * 4

            ColumnLayout {
                id: informationColumnLayout
                anchors.fill: parent
                spacing: app.margins

                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: playerPropertyLayout.height + 2 * app.margins
                    color: "transparent"
                    border.color: "white"
                    border.width: app.borderWidth

                    Column {
                        id: playerPropertyLayout
                        spacing: app.margins / 2
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: parent.left
                        anchors.leftMargin: app.margins

                        Label {
                            font.bold: true
                            font.family: app.fontFamily
                            font.pixelSize: app.mediumFont
                            font.weight: Font.ExtraBold
                            z: Map.Layer4Highest
                            color: "white"
                            text: qsTr("Experience") + ": " + Game.world.player.experience
                        }

                        Label {
                            font.bold: true
                            font.family: app.fontFamily
                            font.pixelSize: app.mediumFont
                            font.weight: Font.ExtraBold
                            z: Map.Layer4Highest
                            color: "white"
                            text: qsTr("Health") + ": " + Game.world.player.health  + "/" + Game.world.player.healthMax
                        }

                        Label {
                            font.bold: true
                            font.family: app.fontFamily
                            font.pixelSize: app.mediumFont
                            font.weight: Font.ExtraBold
                            z: Map.Layer4Highest
                            color: "white"
                            text: qsTr("Mana") + ": " + Game.world.player.mana  + "/" + Game.world.player.manaMax
                        }

                        Label {
                            font.bold: true
                            font.family: app.fontFamily
                            font.pixelSize: app.mediumFont
                            font.weight: Font.ExtraBold
                            z: Map.Layer4Highest
                            color: "white"
                            text: qsTr("Wisdom") + ": " + Game.world.player.wisdom
                        }

                        Label {
                            font.bold: true
                            font.family: app.fontFamily
                            font.pixelSize: app.mediumFont
                            font.weight: Font.ExtraBold
                            z: Map.Layer4Highest
                            color: "white"
                            text: qsTr("Strength") + ": " + Game.world.player.strength
                        }

                        Label {
                            font.bold: true
                            font.family: app.fontFamily
                            font.pixelSize: app.mediumFont
                            font.weight: Font.ExtraBold
                            z: Map.Layer4Highest
                            color: "white"
                            text: qsTr("Stealth") + ": " + Game.world.player.stealth
                        }
                    }
                }

                Item {
                    id: itemDescription
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

        InventoryItem {
            id: inventoryItem
            Layout.fillHeight: true
            Layout.fillWidth: true
            cellSize: root.cellSize
        }
    }
}
