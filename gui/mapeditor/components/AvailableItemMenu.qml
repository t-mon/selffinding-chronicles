import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.0
import QtQuick.Controls.Material 2.0
import Qt.labs.folderlistmodel 2.1

import Box2D 2.0
import Chronicles 1.0

ColumnLayout {
    id: root
    anchors.fill: parent
    clip: true

    TabBar {
        id: optionsTabBar
        Layout.fillWidth: true

        onCurrentIndexChanged: {
            switch(currentIndex) {
            case 0:
                Game.mapEditor.mode = GameMapEditor.ModeItems
                break;
            case 1:
                Game.mapEditor.mode = GameMapEditor.ModeObjects
                break;
            case 2:
                Game.mapEditor.mode = GameMapEditor.ModeCharacters
                break;
            case 3:
                Game.mapEditor.mode = GameMapEditor.ModeEnemies
                break;
            }
        }

        TabButton { text: qsTr("Items") }
        TabButton { text: qsTr("Objects") }
        TabButton { text: qsTr("Characters") }
        TabButton { text: qsTr("Enemies") }
    }

    StackLayout {
        id: mainControlsLayout
        width: parent.width
        Layout.fillWidth: true
        Layout.fillHeight: true
        currentIndex: optionsTabBar.currentIndex

        Item {
            id: availableItems
            Layout.fillWidth: true
            Layout.fillHeight: true

            ListView {
                id: itemsListView
                anchors.fill: parent
                clip: true
                ScrollBar.vertical: ScrollBar { }

                model: Game.mapEditor.availableItems
                delegate: ItemDelegate {
                    width: parent.width
                    highlighted: ListView.isCurrentItem
                    onClicked: itemsListView.currentIndex = index

                    RowLayout {
                        anchors.fill: parent
                        anchors.leftMargin: app.margins
                        anchors.verticalCenter: parent.verticalCenter

                        Image {
                            Layout.preferredWidth: parent.height * 0.8
                            Layout.preferredHeight: parent.height * 0.8
                            Layout.alignment: Qt.AlignVCenter
                            source: dataDirectory + model.imageName
                        }

                        Label {
                            Layout.fillWidth: true
                            Layout.alignment: Qt.AlignVCenter
                            text: model.name
                            color: itemsListView.currentIndex === index ? "black" : "white"
                        }
                    }
                }

                onCurrentIndexChanged: Game.mapEditor.createSelectedGameItem(Game.mapEditor.availableItems.get(itemsListView.currentIndex).resourcePath)
            }
        }

        Item {
            id: objectsItems
            Layout.fillWidth: true
            Layout.fillHeight: true

            ListView {
                id: objectsListView
                anchors.fill: parent
                clip: true
                ScrollBar.vertical: ScrollBar { }

                model: Game.mapEditor.availableObjects
                delegate: ItemDelegate {
                    width: parent.width
                    highlighted: ListView.isCurrentItem
                    onClicked: objectsListView.currentIndex = index

                    RowLayout {
                        anchors.fill: parent
                        anchors.leftMargin: app.margins
                        anchors.verticalCenter: parent.verticalCenter

                        Image {
                            Layout.preferredWidth: parent.height * 0.8
                            Layout.preferredHeight: parent.height * 0.8
                            Layout.alignment: Qt.AlignVCenter
                            source: dataDirectory + model.imageName
                        }

                        Label {
                            Layout.fillWidth: true
                            Layout.alignment: Qt.AlignVCenter
                            text: model.name
                            color: objectsListView.currentIndex === index ? "black" : "white"
                        }
                    }
                }

                //onCurrentIndexChanged: Game.mapEditor.createSelectedGameObject(Game.mapEditor.availableItems.get(itemsListView.currentIndex).resourcePath)
            }
        }


        Item {
            id: availableCharacters
            Layout.fillWidth: true
            Layout.fillHeight: true

            ListView {
                id: charactersListView
                anchors.fill: parent
                clip: true
                ScrollBar.vertical: ScrollBar { }

                model: Game.mapEditor.availableCharacters
                delegate: ItemDelegate {
                    width: parent.width
                    highlighted: ListView.isCurrentItem
                    onClicked: charactersListView.currentIndex = index

                    RowLayout {
                        anchors.fill: parent
                        anchors.leftMargin: app.margins
                        anchors.verticalCenter: parent.verticalCenter

                        Image {
                            Layout.preferredWidth: parent.height * 0.8
                            Layout.preferredHeight: parent.height * 0.8
                            Layout.alignment: Qt.AlignVCenter
                            source: dataDirectory + model.imageName
                        }

                        Label {
                            Layout.fillWidth: true
                            Layout.alignment: Qt.AlignVCenter
                            text: model.name
                            color: charactersListView.currentIndex === index ? "black" : "white"
                        }
                    }
                }

                onCurrentIndexChanged: Game.mapEditor.createSelectedGameItem(Game.mapEditor.availableCharacters.get(charactersListView.currentIndex).resourcePath)
            }
        }
    }
}
