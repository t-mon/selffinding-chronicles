import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.0
import QtQuick.Controls.Material 2.0

import Box2D 2.0
import Chronicles 1.0

import "../components"
import "../gameitems"
import "../gameoverlays"
import "../physics"

GamePage {
    id: gamePage

    title: qsTr("Map editor")

    header: ToolBar {
        background: Rectangle {
            implicitHeight: 40
            color: Material.color(Material.BlueGrey)
        }

        RowLayout {
            anchors.fill: parent
            HeaderButton {
                imageSource: dataDirectory + "/icons/back.svg"
                onClicked: pageStack.pop()
            }
        }
    }

    Component.onCompleted: {
        console.log("Game scene size:", gamePage.width, "/", gamePage.height, "grid size:", app.gridSize)
        forceActiveFocus()
    }

    property var currentItem: Game.mapEditor.availableItems.get(itemsListView.currentIndex)

    Item {
        id: sceneItem
        anchors.fill: parent

        RowLayout {
            id: mainLayout
            anchors.fill: parent

            Item {
                Layout.fillHeight: true
                Layout.preferredWidth: app.gridSize * 10

                ColumnLayout {
                    anchors.fill: parent
                    ListView {
                        id: itemsListView
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        ScrollBar.vertical: ScrollBar { }

                        model: Game.mapEditor.availableItems
                        delegate: ItemDelegate {
                            width: parent.width
                            highlighted: ListView.isCurrentItem
                            onClicked: itemsListView.currentIndex = index

                            GameLabel {
                                Layout.alignment: Qt.AlignVCenter
                                Layout.fillWidth: true
                                text: model.name
                                color: itemsListView.currentIndex === index ? "black" : "white"
                            }
                        }
                    }

                    Rectangle {
                        id: itemDescription
                        Layout.fillWidth: true
                        Layout.preferredHeight: 5 * app.gridSize
                        color: "darkgray"

                        RowLayout {
                            Layout.fillWidth: true

                            ContentItemImage {
                                id: gameItemImage
                                Layout.preferredWidth: app.menuItemSize
                                Layout.preferredHeight: app.menuItemSize
                                imageSource: currentItem ? dataDirectory + currentItem.imageName : ""
                            }

                            GameLabel {
                                Layout.fillWidth: true
                                Layout.alignment: Qt.AlignVCenter
                                text: (currentItem ? currentItem.name : "")
                                font.pixelSize: largeFont
                                color: "white"
                            }
                        }

                    }
                }

            }

            // World flickble
            Flickable {
                id: worldFlickable
                Layout.fillHeight: true
                Layout.fillWidth: true
                clip: true
                contentWidth: worldItem.width
                contentHeight: worldItem.height

                ScrollBar.vertical: ScrollBar { }
                ScrollBar.horizontal: ScrollBar { }

                MouseArea {
                    anchors.fill: parent

                    onXChanged: {

                    }

                    onYChanged: {

                    }
                }

                Item {
                    id: worldItem
                    width: 100 * app.gridSize
                    height: 100 * app.gridSize

                    Rectangle {
                        anchors.fill: parent
                        color: "lightgray"
                    }
                }
            }

        }
    }
}
