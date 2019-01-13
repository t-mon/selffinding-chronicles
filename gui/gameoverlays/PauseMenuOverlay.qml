import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0

import Chronicles 1.0

import "../components"

GameOverlayItem {
    id: root

    Rectangle {
        id: backgroundRectangle
        anchors.fill: parent
        color: "black"
        opacity: 0.7
    }


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
            onClicked: Game.world.resumeGame()
        }

        GameButton {
            Layout.fillWidth: true
            text: qsTr("Settings")
            font.pixelSize: app.largeFont
            onClicked: {
                pageStack.push(Qt.resolvedUrl("../SettingsPage.qml"))
            }
        }

        GameButton {
            Layout.fillWidth: true
            text: Game.debugging ? qsTr("Debug mode OFF") : qsTr("Debug mode ON")
            font.pixelSize: app.largeFont
            onClicked: {
                Game.debugging = !Game.debugging
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
                pageStack.pop()
            }
        }
    }

}
