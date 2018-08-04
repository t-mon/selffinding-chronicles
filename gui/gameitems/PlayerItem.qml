import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0

import Chronicles 1.0

Item {
    id: root

    rotation: Game.world.player.angle * 180 / Math.PI + 90

    Rectangle {
        id: auraCircle
        anchors.centerIn: parent
        border.color: Qt.rgba(0, 0, 0, 0.2)
        border.width: 1
        radius: width / 2
        visible: Game.debugging
        color: "transparent"
        width: root.width * Game.world.player.auraRange
        height: root.width * Game.world.player.auraRange
    }

    Image {
        id: playerImage
        anchors.fill: parent
        source: dataDirectory + "/images/characters/player.svg"
    }

}
