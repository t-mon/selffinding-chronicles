import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0

import Chronicles 1.0

import "../components"

Item {
    id: root

    GameOverlayButton {
        id: menuButton
        anchors.left: parent.left
        anchors.leftMargin: app.margins
        anchors.bottom: parent.bottom
        anchors.bottomMargin: app.margins
        width: app.gridSize * 2
        height: width
        iconSource: dataDirectory + "/icons/settings.svg"
        onClicked: Game.world.pauseGame()
    }

}
