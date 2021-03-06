import QtQuick 2.9
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
        onClicked: Game.engine.pauseGame()
    }

    EquipmentContentItem {
        id: equipmentRow
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: app.margins
    }

    PlayerStatusItem {
        id: playerStatus
        anchors.right: parent.right
        anchors.rightMargin: app.margins
        anchors.bottom: parent.bottom
        anchors.bottomMargin: app.margins

        height: app.gridSize * 1.5
        width: app.gridSize * 6
    }

}
