import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0

import Chronicles 1.0

Item {
    id: root

    Rectangle {
        anchors.centerIn: parent
        border.color: Qt.rgba(0, 0, 0, 0.2)
        border.width: 1
        radius: width / 2
        color: "transparent"
        width: root.width * 3
        height: root.width * 3
    }

    Image {
        id: playerImage
        anchors.fill: parent
        source: "/characters/player.svg"
    }

    // Moving animation
    Behavior on x { NumberAnimation { duration: Game.intervall } }
    Behavior on y { NumberAnimation { duration: Game.intervall } }
}
