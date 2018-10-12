import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0

import Chronicles 1.0

import "components"

Page {
    id: root

    Component.onDestruction: {
        Game.running = false
    }

    GameScene {
        id: gameScene
        anchors.fill: parent
        visible: !Game.world.loading
    }

    RowLayout {
        anchors.left: parent.left
        anchors.leftMargin: app.margins
        anchors.top: parent.top
        anchors.topMargin: app.margins

        Item {
            id: coordinateSystemImage
            opacity: Game.debugging ? 1 : 0
            Layout.preferredHeight: app.gridSize * 3
            Layout.preferredWidth: app.gridSize * 3
            Image {
                anchors.fill: parent
                source: dataDirectory + "/images/game/koordinate-system.png"
            }
        }
    }

    RowLayout {
        id: bottomLeftOptions
        anchors.left: parent.left
        anchors.leftMargin: app.margins
        anchors.bottom: parent.bottom
        anchors.bottomMargin: app.margins

        Rectangle {
            id: pauseButton
            height: 40
            width: height
            radius: height * 0.2

            Label {
                anchors.centerIn: parent
                text: "II"
            }

            MouseArea {
                anchors.fill: parent
                onClicked: menuPopup.open()
            }
        }
    }


    Rectangle {
        id: loadingScreen
        anchors.fill: parent

        visible: Game.world.loading

        Column {
            id: loadingColumn
            anchors.centerIn: parent

            GameLabel {
                id: loadingLabel
                anchors.horizontalCenter: parent.horizontalCenter
                text: "Loading..."
                font.pixelSize: app.largeFont
            }

            BusyIndicator {
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }
    }

    Popup {
        id: menuPopup
        x: root.width * 0.15
        y: root.height * 0.1
        width: root.width * 0.7
        height: root.height * 0.8
        modal: true
        focus: true

        contentItem: Item {
            ColumnLayout {
                anchors.fill: parent
                anchors.margins: app.margins

                GameLabel {
                    text: "Game paused"
                    font.pixelSize: app.largeFont
                    Layout.alignment: Qt.AlignHCenter
                }

                Button {
                    Layout.fillWidth: true
                    text: qsTr("Main menu")
                    onClicked: {
                        menuPopup.close()
                        pageStack.pop()
                    }
                }

                Button {
                    Layout.fillWidth: true
                    text: qsTr("Settings")
                    onClicked: {
                        menuPopup.close()
                        pageStack.push(Qt.resolvedUrl("SettingsPage.qml"))
                    }
                }

                Button {
                    Layout.fillWidth: true
                    text: qsTr("Close")
                    onClicked: {
                        Game.running = true
                        menuPopup.close()
                    }
                }
            }
        }

        onOpened: Game.running = false
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent
    }

}
