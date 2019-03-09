import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.0

import Box2D 2.0
import Chronicles 1.0

import "components"
import "gameitems"
import "gameoverlays"
import "physics"

GamePage {
    id: gamePage

    title: qsTr("Map editor")

    header: ToolBar {
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

    Item {
        id: sceneItem
        anchors.fill: parent

        RowLayout {
            id: mainLayout
            anchors.fill: parent

            ListView {
                id: itemsListView
                Layout.fillHeight: true
                Layout.preferredWidth: app.gridSize * 10

                model: Game.mapEditor.availableItems
                delegate: ItemDelegate {
                    width: parent.width

                    height: app.gridSize * 3
                    highlighted: ListView.isCurrentItem
                    onClicked: itemsListView.currentIndex = index

                    RowLayout {
                        anchors.fill: parent

                        Item {
                            Layout.preferredHeight: app.gridSize * 3
                            Layout.preferredWidth: app.gridSize * 3

                            Rectangle {
                                anchors.fill: parent
                                color: "white"
                                border.color: "black"
                                border.width: 1
                            }

                            Image {
                                id: itemImage
                                anchors.fill: parent
                                anchors.margins: app.margins
                                source: dataDirectory + model.imageName
                            }
                        }

                        GameLabel {
                            Layout.alignment: Qt.AlignVCenter
                            Layout.fillWidth: true
                            text: model.name
                            color: itemsListView.currentIndex === index ? "black" : "white"
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

                Item {
                    id: worldItem
                    width: 100 * app.gridSize
                    height: 100 * app.gridSize

                    //                    Rectangle {
                    //                        anchors.fill: parent
                    //                        color: "steelblue"
                    //                    }

                    RadialGradient {
                        anchors.fill: parent
                        gradient: Gradient {

                            GradientStop {
                                position: 0.000
                                color: Qt.rgba(1, 0, 0, 1)
                            }
                            GradientStop {
                                position: 0.167
                                color: Qt.rgba(1, 1, 0, 1)
                            }
                            GradientStop {
                                position: 0.333
                                color: Qt.rgba(0, 1, 0, 1)
                            }
                            GradientStop {
                                position: 0.500
                                color: Qt.rgba(0, 1, 1, 1)
                            }
                            GradientStop {
                                position: 0.667
                                color: Qt.rgba(0, 0, 1, 1)
                            }
                            GradientStop {
                                position: 0.833
                                color: Qt.rgba(1, 0, 1, 1)
                            }
                            GradientStop {
                                position: 1.000
                                color: Qt.rgba(1, 0, 0, 1)
                            }
                        }
                    }

                }
            }

        }
    }
}
