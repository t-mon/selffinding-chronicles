import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0

import Chronicles 1.0

import "components"

GamePage {
    id: root

    Connections {
        target: Game.world
        onLoadingChanged: {
            if (!loading) {
                console.log("loading finished")
                pageStack.push(Qt.resolvedUrl("GamePage.qml"), { replace: true })
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

