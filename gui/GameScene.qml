import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.0

import Chronicles 1.0

import "components"

Item {
    id: root

    property real zoomLevel: 1.0
    property real cellSize: zoomLevel * Math.min(root.width / 30, root.height / 20)

    property bool debugView: true

    Component.onCompleted: {
        console.log("Game scene size: " + root.width + "/" + root.height)
        forceActiveFocus()
    }

    Keys.onPressed: Game.keyPressed(event.key, event.isAutoRepeat)
    Keys.onReleased: Game.keyReleased(event.key, event.isAutoRepeat)

    Connections {
        target: Game
        onTick: {
            calculateAngle()
        }
        onPaintEvent: {
            //console.log("paint event")
        }
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

        enabled: false
        antialiasing: true

        onContentXChanged: {
            // Calculate the new screen position
            var contentOffsetX = parseInt(contentX / cellSize)
            var contentOffsetY = parseInt(contentY / cellSize)
            Game.world.currentViewOffset = Qt.point(contentOffsetX, contentOffsetY)
        }

        onContentYChanged: {
            // Calculate the new screen position
            var contentOffsetX = parseInt(contentX / cellSize)
            var contentOffsetY = parseInt(contentY / cellSize)
            Game.world.currentViewOffset = Qt.point(contentOffsetX, contentOffsetY)
        }

        Item {
            id: worldBackground
            width: Game.world.map.width * cellSize
            height: Game.world.map.height * cellSize
            z: 0

            Repeater {
                id: fieldRepeater
                model: Game.world
                delegate: FieldItem {
                    width: cellSize
                    height: cellSize
                    field: Game.world.get(model.index)
                    x: field.position.x * cellSize
                    y: field.position.y * cellSize
                }
            }

            //            Canvas {
            //                id: gridCanvas
            //                anchors.fill: parent
            //                smooth: true
            //                antialiasing: true

            //                onPaint: {
            //                    var ctx = gridCanvas.getContext("2d")
            //                    ctx.strokeStyle = Qt.rgba(0, 0, 0, 0.5);
            //                    ctx.lineWidth = 1
            //                    ctx.save()
            //                    ctx.reset()

            //                    for (var i = 0; i < Game.world.size.height; i++) {
            //                        ctx.moveTo(i * cellSize, 0)
            //                        ctx.lineTo(i * cellSize, Game.world.size.width * cellSize)
            //                    }

            //                    for (var j = 0; j < Game.world.size.width; j++) {
            //                        ctx.moveTo(0, j * cellSize)
            //                        ctx.lineTo(Game.world.size.height * cellSize, j * cellSize)
            //                    }

            //                    ctx.stroke()
            //                    ctx.restore()
            //                }
            //            }

            Rectangle {
                id: boundingRectangle
                color: "white"
                border.color: "red"
                border.width: 3
                antialiasing: true
                opacity: Game.debugging ? 0.2 : 0
                width: Game.world.boundingSize.width * cellSize
                height: Game.world.boundingSize.height * cellSize


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

//            Image {
//                anchors.fill: boundingRectangle
//                source: "/images/world/trees/tree-1.png"
//                opacity: 1
//            }


            PlayerItem {
                id: playerItem
                width: root.cellSize * Game.world.player.size.width
                height: root.cellSize * Game.world.player.size.height

                antialiasing: true

                x: Game.world.player.position.x * cellSize - cellSize / 2
                y: Game.world.player.position.y * cellSize - cellSize / 2
                z: 1

                onXChanged: evaluateBoundingRectangle()
                onYChanged: evaluateBoundingRectangle()

                Component.onCompleted: evaluateBoundingRectangle()
            }

            GameLabel {
                id: nameLabel
                anchors.horizontalCenter: playerItem.horizontalCenter
                anchors.bottom: playerItem.top
                anchors.bottomMargin: 4
                visible: Game.debugging
                text: Game.world.player.name
            }
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
        id: sceenMouseArea
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

        var dx = (worldFlickable.contentX + sceenMouseArea.mouseX) - Game.world.player.position.x * cellSize
        var dy = (worldFlickable.contentY + sceenMouseArea.mouseY) - Game.world.player.position.y * cellSize
        //console.log("--> ",  dy, " | ", dx)
        Game.world.player.angle = Math.atan2(dy , dx)
    }
}
