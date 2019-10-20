import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0

import Chronicles 1.0

import "../components"

GameOverlayItem {
    id: root

    property ChestItem chestItem: Game.engine.currentChestItem

    Item {
        anchors.fill: parent
        anchors.margins: app.gridSize * 3

        Rectangle {
            anchors.fill: parent
            color: "#55000000"
            radius: app.gridSize
        }


        ColumnLayout {
            anchors.fill: parent
            anchors.margins: app.margins

            Item {
                Layout.fillWidth: true
                Layout.fillHeight: true

                ColumnLayout {
                    anchors.fill: parent

                    GameLabel {
                        Layout.fillWidth: true
                        Layout.alignment: Qt.AlignHCenter
                        horizontalAlignment: Text.AlignHCenter
                        text: qsTr("Unlocking chest")
                        font.pixelSize: app.largeFont
                        color: "white"
                    }

                    GameLabel {
                        Layout.fillWidth: true
                        Layout.alignment: Qt.AlignHCenter
                        horizontalAlignment: Text.AlignHCenter
                        text: chestItem ? chestItem.unlockProgress : ""
                        color: "white"
                    }


                    Item {
                        Layout.fillWidth: true
                        Layout.preferredHeight: app.gridSize * 3

                        RowLayout {
                            anchors.fill: parent
                            spacing: app.margins

                            GameButton {
                                Layout.fillHeight: true
                                Layout.preferredWidth: height
                                visible: Game.engine.playerController.controlMode === PlayerController.ControlModeTouchscreen
                                text: qsTr("L")
                                onClicked: Game.engine.unlockLeftClicked()
                            }

                            Rectangle {
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                                radius: app.gridSize
                                color: "black"

                                ProgressBar {
                                    anchors.centerIn: parent
                                    from: 0
                                    to: 100
                                    value: chestItem ? chestItem.unlockProgress : 0
                                }
                            }

                            GameButton {
                                Layout.fillHeight: true
                                Layout.preferredWidth: height
                                visible: Game.engine.playerController.controlMode === PlayerController.ControlModeTouchscreen
                                text: qsTr("R")
                                onClicked: Game.engine.unlockRightClicked()
                            }

                        }
                    }
                }
            }

            GameButton {
                Layout.fillWidth: true
                text: qsTr("Give up")
                onClicked: { Game.engine.giveUpUnlocking() }
            }
        }
    }
}
