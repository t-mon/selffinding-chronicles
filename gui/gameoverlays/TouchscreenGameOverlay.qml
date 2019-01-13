import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0

import Chronicles 1.0

import "../components"

Item {
    id: root

    property bool joyStickVisible: joystickController.visible

    function reset() {
        console.log("Reset joystick")
        joystickController.reset()
        joystickController.visible = false
    }

    MultiPointTouchArea {
        id: touchscreenMouseArea
        anchors.fill: parent
        maximumTouchPoints: 1

        touchPoints: [
            TouchPoint {
                id: touchPoint
                onXChanged: {
                    if (root.joyStickVisible) {
                        var dx = x - touchscreenMouseArea.joystickCenterX
                        var dy = y - touchscreenMouseArea.joystickCenterY
                        joystickController.processMousePosition(dx, dy)
                    }
                }

                onYChanged: {
                    if (root.joyStickVisible) {
                        var dx = x - touchscreenMouseArea.joystickCenterX
                        var dy = y - touchscreenMouseArea.joystickCenterY
                        joystickController.processMousePosition(dx, dy)
                    }
                }
            }
        ]

        property real joystickCenterX: 0
        property real joystickCenterY: 0

        onPressed: {
            console.log("Pressed", touchPoint.x, touchPoint.y)
            touchscreenMouseArea.joystickCenterX = touchPoint.x
            touchscreenMouseArea.joystickCenterY = touchPoint.y

            joystickController.x = touchPoint.x - joystickController.width / 2
            joystickController.y = touchPoint.y - joystickController.height / 2
            joystickController.visible = true
        }

        onReleased: {
            console.log("Touchscreen released")
            joystickController.processMousePosition(0, 0)
            joystickController.visible = false
        }
    }

    GameOverlayButton {
        id: menuButton
        anchors.right: parent.right
        anchors.rightMargin: app.margins
        anchors.top: parent.top
        anchors.topMargin: app.margins
        width: app.gridSize * 2
        height: width
        iconSource: dataDirectory + "/icons/settings.svg"
        onClicked: Game.world.pauseGame()
    }

    GameOverlayButton {
        id: inventoryButton
        anchors.left: parent.left
        anchors.leftMargin: app.margins
        anchors.top: parent.top
        anchors.topMargin: app.margins
        width: app.gridSize * 2
        height: width
        iconSource: dataDirectory + "/icons/info.svg"
        onClicked: Game.world.inventoryClicked()
    }

    JoystickControllerItem {
        id: joystickController
        width: app.gridSize * 4
        height: width
        visible: false
    }

    GameOverlayButton {
        id: primaryActionButton
        anchors.right: parent.right
        anchors.rightMargin: app.margins + app.gridSize
        anchors.bottom: parent.bottom
        anchors.bottomMargin: app.gridSize * 4 + app.margins
        width: app.gridSize * 2.5
        height: width

        onClicked: {
            console.log("A clicked")
            Game.world.playerController.clickPrimaryAction()
        }

        GameLabel {
            anchors.centerIn: parent
            text: "A"
            color: "white"
        }
    }

    GameOverlayButton {
        id: secondaryActionButton
        anchors.right: parent.right
        anchors.rightMargin: app.gridSize * 3 + app.margins
        anchors.bottom: parent.bottom
        anchors.bottomMargin: app.margins + app.gridSize
        width: app.gridSize * 2.5
        height: width

        onClicked: {
            console.log("B clicked")
            Game.world.playerController.clickSecondaryAction()
        }

        GameLabel {
            anchors.centerIn: parent
            text: "B"
            color: "white"
        }
    }

}
