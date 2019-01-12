import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0

import Chronicles 1.0

import "../components"

GameOverlayPage {
    id: root

    property ChestItem chestItem: Game.world.currentChestItem

    Rectangle {
        anchors.fill: parent
        color: "black"
        opacity: 0.4
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

                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: app.gridSize * 3
                    radius: app.gridSize
                    color: "black"

                    ProgressBar {
                        anchors.centerIn: parent
                        from: 0
                        to: 100
                        value: chestItem ? chestItem.unlockProgress : 0
                    }
                }
            }
        }

        Item {
            Layout.alignment: Qt.AlignHCenter
            Layout.preferredWidth: root.width * 2 / 3
            Layout.preferredHeight: app.gridSize * 3

            RowLayout {
                anchors.left: parent.left
                anchors.right: parent.right

                GameButton {
                    Layout.fillWidth: true
                    text: qsTr("Give up")
                    onClicked: { Game.world.giveUpUnlocking() }
                }
            }

            //Rectangle {anchors.fill: parent; color: "blue" }
        }
    }


}
