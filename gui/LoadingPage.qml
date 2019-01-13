import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0

import Chronicles 1.0

import "components"

GamePage {
    id: root

    Component.onCompleted: {
        if (Game.world.loaded) {
            console.log("Loading already finished")
            pageStack.replace(root, Qt.resolvedUrl("GamePage.qml"))
        }
    }

    Connections {
        target: Game.world
        onLoadingChanged: {
            if (!loading) {
                console.log("Loading finished")
                pageStack.replace(root, Qt.resolvedUrl("GamePage.qml"))
            }
        }
    }

    Rectangle {
        id: loadingScreen
        anchors.fill: parent
        color: app.backgroundColor

        Column {
            id: loadingColumn
            anchors.centerIn: parent

            GameLabel {
                id: loadingLabel
                anchors.horizontalCenter: parent.horizontalCenter
                text: "Loading..."
                font.pixelSize: app.largeFont
                color: "white"
            }

            GameBusyIndicator {
                anchors.horizontalCenter: parent.horizontalCenter
                running: true
            }
        }
    }
}

