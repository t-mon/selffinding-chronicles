import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0

import Chronicles 1.0
import "components"

Page {
    id: root

    header: ChroniclesHeader {
        text: qsTr("%1").arg(app.title)
        backButtonVisible: true
        onBackPressed: pageStack.pop()
    }

    GameScene {
        id: gameScene
        anchors.fill: parent

        Keys.onPressed: Game.keyPressed(event.key)
        Keys.onReleased: Game.keyReleased(event.key)
    }

    Component.onCompleted: {
        Game.running = true
        print("Game page created")
        focus = true
    }

    Component.onDestruction: Game.running = false
}
