import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0

import Chronicles 1.0

import "../components"

Item {
    id: root

    Rectangle {
        anchors.fill: parent
        color: "#c2c2c2ff"
    }

    ColumnLayout {
        anchors.fill: parent

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
                model: Game.world.player.inventory
                delegate: Rectangle {
                    width: inventoryGridView.width / 6
                    height: width
                    color: "steelblue"

                    Image {
                        id: itemImage
                        anchors.fill: parent
                        source: dataDirectory + model.imageName
                        opacity: Game.debugging ? 0.5 : 1
                    }

                }
            }


            //            Item {
            //                Layout.fillWidth: true
            //                Layout.fillHeight: true

            //                GridLayout {
            //                    id: inventoryGridView
            //                    anchors.fill: parent
            //                    columns: 6

            //                    Repeater {
            //                        id: itemsRepeater
            //                        model: Game.world.player.inventory
            //                        delegate: Rectangle {
            //                            width: inventoryGridView.width / 6
            //                            height: width
            //                            color: "steelblue"

            //                            Image {
            //                                id: itemImage
            //                                anchors.fill: parent
            //                                source: dataDirectory + model.imageName
            //                                opacity: Game.debugging ? 0.5 : 1
            //                            }

            //                        }
            //                    }

            //                }
            //            }

            Rectangle {
                Layout.minimumWidth: root.width * 0.2
                Layout.fillHeight: true
                color: "yellow"; opacity: 0.1;
            }
        }

        RowLayout {

            Rectangle {
                Layout.fillWidth: true
                Layout.minimumHeight: root.height * 0.1
                color: "green"; opacity: 0.1;
            }
        }
    }
}
