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
        Game.mapEditor.loadAvailableGameItems()
        Game.mapEditor.onEditorViewSizeChanged(Qt.size(gamePage.width, gamePage.height))
    }

    property var currentItem: Game.mapEditor.availableItems.get(itemsListView.currentIndex)

    // Pysical world
    World {
        id: physicsWorld
        gravity: Qt.point(0, 0)
        //onStepped: Game.onTick()
        running: true
    }

    Item {
        id: sceneItem
        anchors.fill: parent

        RowLayout {
            id: mainLayout
            anchors.fill: parent

            Item {
                id: mainControlsItem
                Layout.fillHeight: true
                Layout.preferredWidth: 300
                z: 1

                ColumnLayout {
                    id: mainControlsColumn
                    anchors.fill: parent

                    TabBar {
                        id: optionsTabBar
                        Layout.fillWidth: true
                        TabButton { text: qsTr("Items") }
                        TabButton { text: qsTr("Visible") }
                        TabButton { text: qsTr("Settings") }
                    }

                    StackLayout {
                        id: mainControlsLayout
                        width: parent.width
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        currentIndex: optionsTabBar.currentIndex

                        Item {
                            id: availableItems
                            anchors.fill: parent

                            ListView {
                                id: itemsListView
                                anchors.fill: parent
                                ScrollBar.vertical: ScrollBar { }

                                model: Game.mapEditor.availableItems
                                delegate: ItemDelegate {
                                    width: parent.width
                                    highlighted: ListView.isCurrentItem
                                    onClicked: itemsListView.currentIndex = index

                                    Label {
                                        anchors.left: parent.left
                                        anchors.leftMargin: app.margins
                                        anchors.verticalCenter: parent.verticalCenter
                                        text: model.name
                                        color: itemsListView.currentIndex === index ? "black" : "white"
                                    }
                                }

                                onCurrentIndexChanged: Game.mapEditor.createSelectedGameItem(Game.mapEditor.availableItems.get(itemsListView.currentIndex).resourcePath)
                            }
                        }

                        Item {
                            id: visibleItems
                            anchors.fill: parent
                        }

                        Item {
                            id: settings
                            anchors.fill: parent

                            property bool itemDebug: false
                            property bool physicsDebug: false
                            property bool gridSnapping: false

                            ColumnLayout {
                                anchors.left: parent.left
                                anchors.right: parent.right
                                anchors.top: parent.top

                                RowLayout {
                                    Layout.fillWidth: true
                                    Layout.leftMargin: app.margins

                                    Label {
                                        Layout.fillWidth: true
                                        Layout.alignment: Qt.AlignVCenter
                                        text: qsTr("Items debug")
                                        color: "white"
                                    }

                                    CheckBox {
                                        onCheckedChanged: settings.itemDebug = checked
                                        Component.onCompleted: checked = settings.itemDebug
                                    }
                                }

                                RowLayout {
                                    Layout.fillWidth: true
                                    Layout.leftMargin: app.margins

                                    Label {
                                        Layout.fillWidth: true
                                        Layout.alignment: Qt.AlignVCenter
                                        text: qsTr("Physics debug")
                                        color: "white"
                                    }

                                    CheckBox {
                                        onCheckedChanged: settings.physicsDebug = checked
                                        Component.onCompleted: checked = settings.physicsDebug
                                    }
                                }


                                RowLayout {
                                    Layout.fillWidth: true
                                    Layout.leftMargin: app.margins

                                    Label {
                                        Layout.fillWidth: true
                                        Layout.alignment: Qt.AlignVCenter
                                        text: qsTr("Grid snapping")
                                        color: "white"
                                    }

                                    CheckBox {
                                        onCheckedChanged: settings.gridSnapping = checked
                                        Component.onCompleted: checked = settings.gridSnapping
                                    }
                                }
                            }
                        }
                    }

                    Rectangle {
                        id: itemDescription
                        Layout.fillWidth: true
                        Layout.preferredHeight: gamePage.height * 0.2
                        color: "darkgray"

                        RowLayout {
                            Layout.fillWidth: true

                            ContentItemImage {
                                id: gameItemImage
                                Layout.preferredWidth: 80
                                Layout.preferredHeight: 80
                                imageSource: currentItem ? dataDirectory + currentItem.imageName : ""
                            }

                            Label {
                                Layout.fillWidth: true
                                Layout.alignment: Qt.AlignVCenter
                                text: (currentItem ? currentItem.name : "")
                                color: "white"
                            }
                        }
                    }
                }
            }


            // World flickble
            Item {
                id: mainItem
                Layout.fillHeight: true
                Layout.fillWidth: true

                function updatePositions() {
                    // TODO: update position for view

                    if (!temporaryItem)
                        return

                    var positionX = worldFlickable.contentX + editorMouseArea.mouseX - temporaryItem.width / 2
                    var positionY = worldFlickable.contentY + editorMouseArea.mouseY - temporaryItem.height / 2

                    if (settings.gridSnapping) {
                        positionX = Math.round(positionX / app.gridSize) * app.gridSize
                        positionY = Math.round(positionY / app.gridSize) * app.gridSize
                    }

                    temporaryItem.x = positionX
                    temporaryItem.y = positionY

                }

                Flickable {
                    id: worldFlickable
                    anchors.fill: parent
                    contentWidth: worldItem.width
                    contentHeight: worldItem.height
                    clip: true

                    ScrollBar.vertical: ScrollBar { policy: ScrollBar.AlwaysOn }
                    ScrollBar.horizontal: ScrollBar { policy: ScrollBar.AlwaysOn }

                    onContentXChanged: mainItem.updatePositions()
                    onContentYChanged: mainItem.updatePositions()

                    Item {
                        id: worldItem
                        width: Game.mapEditor.map.size.width * app.gridSize
                        height: Game.mapEditor.map.size.height * app.gridSize

                        Rectangle {
                            anchors.fill: parent
                            color: Game.mapEditor.dataManager.worldBackgroundColor
                        }

                        WorldBoundaries {
                            id: worldBoundaries
                            worldObject: worldItem
                        }

                        Repeater {
                            id: characersRepeater
                            model: Game.mapEditor.activeCharacters
                            delegate: CharacterItem {
                                character: Game.mapEditor.activeCharacters.get(model.index)
                                itemDebugEnabled: settings.itemDebug
                                width: model.size.width * app.gridSize
                                height: model.size.height * app.gridSize
                                x: model.position.x * app.gridSize
                                y: model.position.y * app.gridSize
                                z: model.layer
                            }
                        }

                        Repeater {
                            id: itemsRepeater
                            model: Game.mapEditor.activeItems
                            delegate: GameItem {
                                gameItem: Game.mapEditor.activeItems.get(model.index)
                                itemDebugEnabled: settings.itemDebug
                                width: model.size.width * app.gridSize
                                height: model.size.height * app.gridSize
                                x: model.position.x * app.gridSize
                                y: model.position.y * app.gridSize
                                z: model.layer
                            }
                        }

                        Repeater {
                            id: enemiesRepeater
                            model: Game.mapEditor.activeEnemies
                            delegate: EnemyItem {
                                itemDebugEnabled: settings.itemDebug
                                enemy: Game.mapEditor.activeEnemies.get(model.index)
                                width: model.size.width * app.gridSize
                                height: model.size.height * app.gridSize
                                x: model.position.x * app.gridSize
                                y: model.position.y * app.gridSize
                                z: model.layer
                            }
                        }

                        GameItem {
                            id: temporaryItem
                            gameItem: Game.mapEditor.selectedGameItem
                            itemDebugEnabled: settings.itemDebug
                            visible: editorMouseArea.containsMouse
                            width: gameItem ? gameItem.size.width * app.gridSize : 0
                            height: gameItem ? gameItem.size.height * app.gridSize : 0
                            z: gameItem ? gameItem.layer : 0
                        }
                    }
                }


                DebugDraw {
                    id: debugDraw
                    world: physicsWorld
                    opacity: 0.4
                    visible: settings.physicsDebug
                }

                MouseArea {
                    id: editorMouseArea
                    anchors.fill: parent
                    hoverEnabled: true
                    //preventStealing: true

                    onMouseXChanged: {
                        console.log("--> mouse position changed", mouseX, mouseY)
                        mainItem.updatePositions()
                    }

                    onMouseYChanged: {
                        console.log("--> mouse position changed", mouseX, mouseY)
                        mainItem.updatePositions()
                    }

                    onClicked: {
                        if (temporaryItem) {
                            var position = Qt.point(temporaryItem.x / app.gridSize, temporaryItem.y / app.gridSize)

                            Game.mapEditor.placeItemOnMap(temporaryItem.gameItem.resourcePath, position)
                        }
                    }
                }
            }
        }
    }
}

