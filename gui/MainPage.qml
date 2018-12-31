import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0

import Chronicles 1.0

import "components"

GamePage {
    id: root

    Component.onCompleted: console.log("Window size " + root.width + "x" + root.height)

    ColumnLayout {
        anchors.centerIn: parent
        GameButton {
            id: playButton
            Layout.alignment: Qt.AlignHCenter
            text: "Play"
            //font.bold: true
            font.pixelSize: app.largeFont
            font.family: app.fontFamily
            onClicked: {
                console.log("Play pressed")
                Game.world.loadMap(":/maps/test-environment.json")
                pageStack.push(Qt.resolvedUrl("LoadingPage.qml"))
            }
        }

        Button {
            id: settingsButton
            Layout.alignment: Qt.AlignHCenter
            text: "Settings"
            font.pixelSize: app.largeFont
            font.family: app.fontFamily
            onClicked: {
                pageStack.push(Qt.resolvedUrl("SettingsPage.qml"))
            }
        }

        Button {
            id: conversationCreatorButton
            Layout.alignment: Qt.AlignHCenter
            text: "Conversation creator"
            font.pixelSize: app.largeFont
            font.family: app.fontFamily
            onClicked: {
                pageStack.push(Qt.resolvedUrl("ConversationCreatorPage.qml"))
            }
        }

        Button {
            id: aboutButton
            Layout.alignment: Qt.AlignHCenter
            text: "About"
            font.pixelSize: app.largeFont
            font.family: app.fontFamily
            onClicked: pageStack.push(Qt.resolvedUrl("AboutPage.qml"))
        }
    }

}
