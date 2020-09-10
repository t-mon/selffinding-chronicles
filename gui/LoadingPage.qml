import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0

import Chronicles 1.0

import "components"

GamePage {
    id: root

    Connections {
        target: Game.engine
        function onLoadedChanged(loaded) {
            if (loaded) {
                console.log("Engine loading finished.")

                console.log("Loading game page async...")
                gamePageLoader.setSource(Qt.resolvedUrl("GameScenePage.qml"))
                gamePageLoader.active = true
            }
        }
    }

    Connections {
        target: gamePageLoader
        function onLoaded() {
            console.log("Game page loading finished. Replace current page on stack.")
            pageStack.replace(root, gamePageLoader.item)
            gamePageLoader.item.runGame()
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

