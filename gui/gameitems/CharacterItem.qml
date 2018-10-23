import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0

import Chronicles 1.0

Item {
    id: root

    rotation: Game.world.player.angle * 180 / Math.PI + 90

    Image {
        id: playerImage
        anchors.fill: parent
        source: dataDirectory + "/images/characters/player.svg"
        opacity: Game.debugging ? 0.5 : 1
    }

    Rectangle {
        id: wireFrame
        anchors.fill: parent
        color: "transparent"
        border.color: "red"
        opacity: 0.3
        border.width: 2
        visible: Game.debugging
        radius: Game.world.player.shape === GameObject.ShapeCircle ? width / 2 : 0
    }
}
