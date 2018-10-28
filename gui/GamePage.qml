import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0

import Chronicles 1.0

import "components"

Page {
    id: root

    Component.onDestruction: {
        Game.running = false
    }

    GameScene {
        id: gameScene
        anchors.fill: parent
        visible: !Game.world.loading
    }

    RowLayout {
        anchors.left: parent.left
        anchors.leftMargin: app.margins
        anchors.top: parent.top
        anchors.topMargin: app.margins

        Item {
            id: coordinateSystemImage
            opacity: Game.debugging ? 1 : 0
            Layout.preferredHeight: app.gridSize * 3
            Layout.preferredWidth: app.gridSize * 3
            Image {
                anchors.fill: parent
                source: dataDirectory + "/images/game/koordinate-system.png"
            }
        }
    }

    Rectangle {
        id: loadingScreen
        anchors.fill: parent

        visible: Game.world.loading

        Column {
            id: loadingColumn
            anchors.centerIn: parent

            GameLabel {
                id: loadingLabel
                anchors.horizontalCenter: parent.horizontalCenter
                text: "Loading..."
                font.pixelSize: app.largeFont
            }

            BusyIndicator {
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }
    }
}
