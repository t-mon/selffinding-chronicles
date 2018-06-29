import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.0

import Chronicles 1.0

Item {
    id: root

    property real zoomLevel: 1.0
    property real cellSize: zoomLevel * Math.min(root.width / Game.world.sceneSize.width, root.height / Game.world.sceneSize.height)

    onCellSizeChanged: Game.world.cellSize = root.cellSize

    Component.onCompleted: {
        console.log("Game scene size: " + root.width + "/" + root.height)
        gridCanvas.requestPaint();
    }

    Connections {
        target: Game
        onTick: calculateAngle()
    }

    Connections {
        target: Game.world.player
        onPositionChanged: {
            evaluateBoundingRectangle()
        }
    }

    Flickable {
        id: worldFlickable
        anchors.fill: parent
        contentWidth: worldBackground.width
        contentHeight: worldBackground.height

        antialiasing: true

        onContentXChanged: console.log("Content x " + contentX)
        onContentYChanged: console.log("Content y " + contentY)

//        // Moving animation
//        Behavior on contentX { NumberAnimation { duration: Game.intervall } }
//        Behavior on contentY { NumberAnimation { duration: Game.intervall } }

        Item {
            id: worldBackground
            width: Game.world.size.width * cellSize
            height: Game.world.size.height * cellSize

            RadialGradient {
                anchors.fill: parent
                gradient: Gradient {
                    GradientStop {
                        position: 0.000
                        color: Qt.rgba(1, 0, 0, 1)
                    }
                    GradientStop {
                        position: 0.167
                        color: Qt.rgba(1, 1, 0, 1)
                    }
                    GradientStop {
                        position: 0.333
                        color: Qt.rgba(0, 1, 0, 1)
                    }
                    GradientStop {
                        position: 0.500
                        color: Qt.rgba(0, 1, 1, 1)
                    }
                    GradientStop {
                        position: 0.667
                        color: Qt.rgba(0, 0, 1, 1)
                    }
                    GradientStop {
                        position: 0.833
                        color: Qt.rgba(1, 0, 1, 1)
                    }
                    GradientStop {
                        position: 1.000
                        color: Qt.rgba(1, 0, 0, 1)
                    }
                }
            }
        }

        Rectangle {
            id: currentField
            color: "green"
            opacity: 0.8
            height: cellSize
            width: cellSize
            x: Game.world.currentPlayerField.x * cellSize
            y: Game.world.currentPlayerField.y * cellSize
        }

        Canvas {
            id: gridCanvas
            anchors.fill: parent
            smooth: true
            onPaint: {
                var ctx = gridCanvas.getContext("2d")
                ctx.strokeStyle = Qt.rgba(0, 0, 0, 0.5);
                ctx.lineWidth = 1
                ctx.save()
                ctx.reset()

                for (var i = 0; i < Game.world.size.height; i++) {
                    ctx.moveTo(i * cellSize, 0)
                    ctx.lineTo(i * cellSize, Game.world.size.width * cellSize)
                }

                for (var j = 0; j < Game.world.size.width; j++) {
                    ctx.moveTo(0, j * cellSize)
                    ctx.lineTo(Game.world.size.height * cellSize, j * cellSize)
                }

                ctx.stroke()
                ctx.restore()
            }
        }

        Rectangle {
            id: boundingRectangle
            color: "white"
            border.color: "red"
            border.width: 3
            opacity: 0.3
            width: Game.world.boundingSize.width * cellSize
            height: Game.world.boundingSize.height * cellSize

            // Moving animation
            Behavior on x { NumberAnimation { duration: Game.intervall } }
            Behavior on y { NumberAnimation { duration: Game.intervall } }

            onXChanged: {
                // Over the right sceen boarder
                if (boundingRectangle.x + boundingRectangle.width >= worldFlickable.contentX + root.width) {
                    var offsetXRight = (boundingRectangle.x + boundingRectangle.width) - (worldFlickable.contentX + root.width)
                    worldFlickable.contentX += offsetXRight
                }

                // Over the left sceen boarder
                if (boundingRectangle.x < worldFlickable.contentX) {
                    var offsetXLeft = boundingRectangle.x - worldFlickable.contentX
                    worldFlickable.contentX += offsetXLeft
                }
            }

            onYChanged: {
                // Over the lower sceen boarder
                if (boundingRectangle.y + boundingRectangle.height >= worldFlickable.contentY + root.height) {
                    var offsetYLower = (boundingRectangle.y + boundingRectangle.height) - (worldFlickable.contentY + root.height)
                    worldFlickable.contentY += offsetYLower
                }

                // Over the upper sceen boarder
                if (boundingRectangle.y < worldFlickable.contentY) {
                    var offsetYUpper = boundingRectangle.y - worldFlickable.contentY
                    worldFlickable.contentY += offsetYUpper
                }
            }
        }

        PlayerItem {
            id: playerItem

            width: cellSize
            height: cellSize

            x: (Game.world.player.position.x * cellSize) - cellSize / 2
            y: (Game.world.player.position.y * cellSize) - cellSize / 2

            rotation: Game.world.player.angle * 180 / Math.PI + 90

            Component.onCompleted: evaluateBoundingRectangle()
        }

        Label {
            id: nameLabel
            anchors.horizontalCenter: playerItem.horizontalCenter
            anchors.bottom: playerItem.top
            anchors.bottomMargin: 4
            text: Game.world.player.name
        }


    }

    function evaluateBoundingRectangle() {

        var worldWidth = Game.world.size.width * cellSize
        var worldHeight = Game.world.size.height * cellSize

        var offsetX = Game.world.player.position.x * cellSize - (boundingRectangle.x + boundingRectangle.width / 2)
        var offsetY = Game.world.player.position.y * cellSize - (boundingRectangle.y + boundingRectangle.height / 2)

        var newPositionX = boundingRectangle.x;
        var newPositionY = boundingRectangle.y;

        // Check east/west border of the world
        if (boundingRectangle.x + offsetX < 0) {
            newPositionX = 0;
        } else if (boundingRectangle.x + boundingRectangle.width + offsetX > worldWidth) {
            newPositionX = worldWidth - boundingRectangle.width;
        } else {
            newPositionX += offsetX;
        }

        // Check north/south border of the world
        if (boundingRectangle.y + offsetY < 0) {
            newPositionY = 0;
        } else if (boundingRectangle.y + boundingRectangle.height + offsetY > worldHeight) {
            newPositionY = worldHeight - boundingRectangle.height;
        } else {
            newPositionY += offsetY;
        }

        boundingRectangle.x = newPositionX
        boundingRectangle.y = newPositionY
    }


    MouseArea {
        id: gameMouseArea
        anchors.fill: parent
        hoverEnabled: true
        visible: Game.controlMode == Game.ControlModeKeyBoardMouse
        preventStealing: Game.controlMode == Game.ControlModeKeyBoardMouse
        onMouseXChanged: calculateAngle()
        onMouseYChanged: calculateAngle()
    }

    function calculateAngle() {
        if (!Game.running || Game.controlMode != Game.ControlModeKeyBoardMouse)
            return;

        var dx = gameMouseArea.mouseX - Game.world.player.position.x
        var dy = gameMouseArea.mouseY - Game.world.player.position.y
        Game.world.player.angle = Math.atan2(dy , dx)
        console.log(dy, " ", dx, " ", Game.world.player.angle)
    }
}
