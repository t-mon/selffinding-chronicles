import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0

import Chronicles 1.0

Page {
    id: root

    ColumnLayout {

        anchors.centerIn: parent



        Button {
            id: playButton
            Layout.alignment: Qt.AlignHCenter
            text: "Play"
            font.bold: true
            font.pixelSize: app.largeFont
            font.family: app.fontFamily
            onClicked: {
                console.log("Play pressed")
                Game.world.loadMap(":/maps/test-map.json")
                pageStack.push(Qt.resolvedUrl("GamePage.qml"))
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
