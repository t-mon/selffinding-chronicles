import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0

import Chronicles 1.0

import "components"

GamePage {
    id: root

    Component.onCompleted: console.log("Window size " + root.width + "x" + root.height)

    GameLabel {
        anchors.left: parent.left
        anchors.leftMargin: app.margins
        anchors.top: parent.top
        anchors.topMargin: app.margins
        text: "v " + gameVersion
        color: "white"

    }

    ColumnLayout {
        anchors.centerIn: parent
        width: parent.width / 3

        GameButton {
            id: playButton
            Layout.fillWidth: true
            text: qsTr("Play")
            font.pixelSize: app.largeFont
            onClicked: {
                console.log("Play pressed")
                Game.world.loadMap(":/maps/test-environment.json")
                pageStack.push(Qt.resolvedUrl("LoadingPage.qml"))
            }
        }

        Item {
            Layout.fillWidth: true
            Layout.preferredHeight: app.gridSize * 2
        }

        GameButton {
            id: settingsButton
            Layout.fillWidth: true
            text: qsTr("Settings")
            onClicked: {
                pageStack.push(Qt.resolvedUrl("SettingsPage.qml"))
            }
        }

        GameButton {
            id: mapEditorButton
            Layout.fillWidth: true
            text: qsTr("Map editor")
            onClicked: {
                pageStack.push(Qt.resolvedUrl("MapEditor.qml"))
            }
        }

        GameButton {
            id: conversationCreatorButton
            Layout.fillWidth: true
            text: qsTr("Conversation creator")
            onClicked: {
                pageStack.push(Qt.resolvedUrl("ConversationCreatorPage.qml"))
            }
        }

        GameButton {
            id: aboutButton
            Layout.fillWidth: true
            text: qsTr("About")
            onClicked: pageStack.push(Qt.resolvedUrl("AboutPage.qml"))
        }
    }

}
