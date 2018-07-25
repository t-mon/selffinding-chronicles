import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0

import Chronicles 1.0
import "components"

Page {
    id: root
    header: ChroniclesHeader {
        text: qsTr("%1").arg(Game.world.map.name)
        backButtonVisible: true
        visible: !Game.world.loading
        onBackPressed: pageStack.pop()
    }

    Component.onDestruction: {
        Game.running = false
    }

    GameScene {
        id: gameScene
        anchors.fill: parent
        visible: !Game.world.loading
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
