import QtQuick 2.9

import Chronicles 1.0

Item {
    id: root

    property real dx: 0
    property real dy: 0
    property real length: 0
    property real angle: 0

    Image {
        id: joystickBaseImage
        anchors.fill: parent
        source: dataDirectory + "/images/game/joystick-base.svg"

        Image {
            id: joystrickButton
            width: root.width * 2 / 5
            height: width
            source: dataDirectory + "/images/game/joystick-button.svg"
        }
    }

    onVisibleChanged: if (visible) reset()

    function reset() {
        joystrickButton.x = root.width / 2 - joystrickButton.width / 2
        joystrickButton.y = root.height / 2 - joystrickButton.height / 2
        processMousePosition(0, 0)

    }

    function processMousePosition(dx, dy) {
        var radius = root.width / 2 - joystrickButton.width / 2
        var normalizedX = dx / radius
        var normalizedY = dy / radius
        var normalizedLength = Math.sqrt(Math.pow(normalizedX, 2) + Math.pow(normalizedY, 2))
        var angle = Math.atan2(normalizedY, normalizedX)

        // Check if we are in range
        if (normalizedLength > 1) {
            normalizedLength = 1
            normalizedX = Math.cos(angle)
            normalizedY = Math.sin(angle)
        }

        root.dx = normalizedX
        root.dy = normalizedY
        root.length = normalizedLength
        root.angle = angle

        Game.engine.playerController.onJoystickChanged(Qt.point(root.dx, root.dy), root.length, root.angle)

        joystrickButton.x = root.width / 2 - joystrickButton.width / 2 + normalizedX * radius
        joystrickButton.y = root.height / 2 - joystrickButton.height / 2 + normalizedY * radius
        //console.log("Joystick", normalizedX, normalizedY, normalizedLength, angle * 180 / Math.PI)
    }

}
