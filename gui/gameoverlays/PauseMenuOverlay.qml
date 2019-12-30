import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0

import Chronicles 1.0

import "../components"

GameOverlayItem {
    id: root

    Rectangle {
        id: menuBackgroundRectangle
        anchors.fill: parent
        anchors.margins: app.gridSize * 3
        color: "#55000000"
        border.color: "white"
        border.width: app.borderWidth
    }

    ColumnLayout {
        anchors.fill: menuBackgroundRectangle
        anchors.margins: app.margins

        spacing: app.margins

        GameLabel {
            Layout.alignment: Qt.AlignHCenter
            text: "Game paused"
            color: "white"
            font.pixelSize: app.largeFont
        }

        GameButton {
            Layout.fillWidth: true
            text: qsTr("Resume game")
            font.pixelSize: app.largeFont
            onClicked: Game.engine.resumeGame()
        }

        GameButton {
            Layout.fillWidth: true
            text: qsTr("Save game")
            font.pixelSize: app.largeFont
            onClicked: {
                worldFlickable.grabToImage(function(result) {
                    //result.saveToFile("/tmp/something.png");
                    Game.engine.dataManager.saveGame(result.image)
                });
            }
        }

        GameButton {
            Layout.fillWidth: true
            text: qsTr("Settings")
            font.pixelSize: app.largeFont
            onClicked: {
                pageStack.push(Qt.resolvedUrl("../SettingsPage.qml"))
            }
        }


        Item {
            id: spacingItem
            Layout.fillWidth: true
            Layout.fillHeight: true
        }

        GameButton {
            Layout.fillWidth: true
            text: qsTr("Exit game")
            font.pixelSize: app.largeFont
            onClicked: {
                gamePageLoader.active = false
                Game.engine.resetEngine()
                pageStack.pop()
            }
        }
    }

}
