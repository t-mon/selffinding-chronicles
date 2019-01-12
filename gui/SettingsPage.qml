import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0

import Chronicles 1.0
import "components"

GamePage {
    id: root

    header: ChroniclesHeader {
        text: qsTr("Settings")
        backButtonVisible: true
        onBackPressed: pageStack.pop()
    }

    Flickable {
        anchors.fill: parent
        contentHeight: settingsColumn.implicitHeight
        anchors.leftMargin: app.margins
        anchors.rightMargin: app.margins


        ColumnLayout {
            id: settingsColumn
            anchors.left: parent.left
            anchors.right: parent.right

            RowLayout {
                Layout.fillWidth: true

                GameLabel {
                    Layout.preferredWidth: settingsColumn.width / 2
                    text: qsTr("Name")
                    color: "white"
                }

                TextField {
                    id: playerNameField
                    text: Game.settings.playerName
                    Layout.fillWidth: true

                    onEditingFinished: {
                        Game.settings.playerName = text
                        Game.world.player.name = text
                    }
                }
            }

            RowLayout {
                Layout.fillWidth: true

                GameLabel {
                    Layout.preferredWidth: settingsColumn.width / 2
                    text: qsTr("Controll mode")
                    color: "white"
                }

                ComboBox {
                    Layout.fillWidth: true
                    model: [qsTr("Keyboard"), qsTr("Keyboard + Mouse"), qsTr("Touchscreen")]
                    currentIndex: {
                        switch (Game.settings.controlMode) {
                        case PlayerController.ControlModeKeyBoard:
                            return 0;
                        case PlayerController.ControlModeKeyBoardMouse:
                            return 1;
                        case PlayerController.ControlModeTouchscreen:
                            return 2;
                        }
                    }

                    onCurrentIndexChanged: {
                        if (currentIndex == 0) {
                            Game.settings.controlMode = PlayerController.ControlModeKeyBoard
                            Game.world.playerController.controlMode = PlayerController.ControlModeKeyBoard
                        } else if (currentIndex == 1) {
                            Game.settings.controlMode = PlayerController.ControlModeKeyBoardMouse
                            Game.world.playerController.controlMode = PlayerController.ControlModeKeyBoardMouse
                        } else if (currentIndex == 2) {
                            Game.settings.controlMode = PlayerController.ControlModeTouchscreen
                            Game.world.playerController.controlMode = PlayerController.ControlModeTouchscreen
                        }
                    }
                }
            }

            RowLayout {
                Layout.fillWidth: true

                GameLabel {
                    Layout.preferredWidth: settingsColumn.width / 2
                    text: qsTr("View mode")
                    color: "white"
                }

                ComboBox {
                    Layout.fillWidth: true
                    model: [qsTr("Windowed"), qsTr("Maximized"), qsTr("Fullscreen")]
                    currentIndex: {
                        switch (Game.settings.visibility) {
                        case ApplicationWindow.Windowed:
                            return 0;
                        case ApplicationWindow.Maximized:
                            return 1;
                        case ApplicationWindow.FullScreen:
                            return 2;
                        }
                    }

                    onCurrentIndexChanged: {
                        switch (currentIndex) {
                        case 0:
                            Game.settings.visibility = ApplicationWindow.Windowed;
                            break;
                        case 1:
                            Game.settings.visibility = ApplicationWindow.Maximized;
                            break;
                        case 2:
                            Game.settings.visibility = ApplicationWindow.FullScreen;
                            break;
                        }
                    }
                }
            }
        }
    }
}
