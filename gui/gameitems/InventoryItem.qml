import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0

import Chronicles 1.0

import "../components"

Item {
    id: root

    Rectangle {
        anchors.fill: parent
        color: "black"
        opacity: 0.4
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        RowLayout {
            Rectangle {
                id: inventoryBackground
                Layout.fillWidth: true
                Layout.minimumHeight: root.height * 0.1
                color: "blue"; opacity: 0.1;
            }
        }

        RowLayout {

            GridView {
                id: inventoryGridView
                Layout.fillWidth: true
                Layout.fillHeight: true
                clip: true
                model: Game.world.player.inventoryProxy
                delegate: inventoryItemDelegate

                Component {
                    id: inventoryItemDelegate

                    Item {
                        width: inventoryGridView.width / 6
                        height: width

                        Rectangle {
                            id: itemRectangle
                            anchors.fill: parent
                            anchors.margins: itemRectangle.width / 8

                            color: "#22FFFFFF"
                            border.color: "gray"
                            border.width: width / 30
                            radius: width / 6

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

            Rectangle {
                Layout.minimumWidth: root.width * 0.2
                Layout.fillHeight: true
                color: "green"; opacity: 0.5;
            }
        }

        RowLayout {

            Rectangle {
                Layout.fillWidth: true
                Layout.minimumHeight: root.height * 0.1
                color: "red"; opacity: 0.5;
            }
        }
    }
}
