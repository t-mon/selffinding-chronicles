import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0

import Chronicles 1.0

import "components"

GamePage {
    id: root

    Component.onCompleted: {
        if (Game.engine.loaded) {
            console.log("Loading already finished")
            pageStack.replace(root, Qt.resolvedUrl("GamePage.qml"))
        }
    }

    Connections {
        target: Game.engine
        onLoadingChanged: {
            if (!loading) {
                console.log("Engine loading finished. Load game page...")
                pageStack.replace(root, Qt.resolvedUrl("Game.qml"))

                //gamePageLoader.setSource(Qt.resolvedUrl("GamePage.qml"))
            }
        }
    }

//    Loader {
//        id: gamePageLoader
//        anchors.fill: parent
//        asynchronous: true
//        onLoaded: {
//            console.log("Game page loading finished.")
//            pageStack.replace(root, gamePageLoader.source)
//        }
//    }

//    MouseArea {
//        anchors.fill: parent
//        onClicked: pageLoader.source = "Page1.qml"
//    }

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

