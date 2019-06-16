import QtQuick 2.9
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

        property real joystickCenterX: 0
        property real joystickCenterY: 0

        touchPoints: [
            TouchPoint {
                id: touchPoint
                onXChanged: {
                    if (root.joyStickVisible) {
                        if (joystickController.offset.x !== 0) {
                            touchscreenMouseArea.joystickCenterX += joystickController.offset.x
                            joystickController.x += joystickController.offset.x
                            joystickController.offset.x = 0
                            touchscreenMouseArea.joystickCenterY += joystickController.offset.y
                            joystickController.y += joystickController.offset.y
                            joystickController.offset.y = 0
                        }

                        var dx = x - touchscreenMouseArea.joystickCenterX
                        var dy = y - touchscreenMouseArea.joystickCenterY
                        joystickController.processMousePosition(dx, dy)
                    }
                }

                onYChanged: {
                    if (root.joyStickVisible) {

                        if (joystickController.offset.y !== 0) {
                            touchscreenMouseArea.joystickCenterX += joystickController.offset.x
                            joystickController.x += joystickController.offset.x
                            joystickController.offset.x = 0
                            touchscreenMouseArea.joystickCenterY += joystickController.offset.y
                            joystickController.y += joystickController.offset.y
                            joystickController.offset.y = 0
                        }

                        var dx = x - touchscreenMouseArea.joystickCenterX
                        var dy = y - touchscreenMouseArea.joystickCenterY
                        joystickController.processMousePosition(dx, dy)
                    }
                }
            }
        ]

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
        onClicked: Game.engine.pauseGame()
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
        onClicked: Game.engine.inventoryClicked()
    }

    PlayerStatusItem {
        id: playerStatus
        anchors.left: inventoryButton.right
        anchors.leftMargin: app.margins
        anchors.verticalCenter: inventoryButton.verticalCenter
        height: app.gridSize * 1.5
        width: app.gridSize * 6
    }

    JoystickControllerItem {
        id: joystickController
        width: app.gridSize * 5
        height: width
        visible: false
//        onXChanged: {
//            // Limit the joystick item to the screen
//            console.log("######### Joystick position changed", joystickController.x, ",", joystickController.y)
//            if (joystickController.x < 0) {
//                joystickController.x = 0
//            }

//            if (joystickController.x > root.width - joystickController.width) {
//                joystickController.x = root.width - joystickController.width
//            }
//        }
//        onYChanged: console.log("######### Joystick position changed", joystickController.x, ",", joystickController.y)
    }

    GameLabel {
        anchors.horizontalCenter: primaryActionButton.horizontalCenter
        anchors.bottom: primaryActionButton.top
        anchors.bottomMargin: app.margins
        font.pixelSize: smallFont
        text: Game.engine.playerFocusItem ? Game.engine.playerFocusItem.interactionString : ""
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
            Game.engine.playerController.clickPrimaryAction()
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
            Game.engine.playerController.clickSecondaryAction()
        }

        GameLabel {
            anchors.centerIn: parent
            text: "B"
            color: "white"
        }
    }

    EquipmentContentItem {
        id: equipmentRow
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: app.margins
    }

}
