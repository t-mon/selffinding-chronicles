    import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.0

import Chronicles 1.0

import "components"
import "gameitems"
import "inventory"

Item {
    id: root

    property real zoomLevel: 1.0
    property real cellSize: zoomLevel * Math.min(root.width / 30, root.height / 20)

    property bool debugView: true
    property bool inventoryVisible: false

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

    Connections {
        target: Game.world.playerController
        onInventoryPressed: {
            console.log("inventory pressed ")
            inventoryItem.visible = !inventoryItem.visible
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

        /* Game Layers
         *      z = 0 -> background, the lowest layer
         *      z = 1 -> plants, and other things the player can walk over
         *      z = 2 -> the player
         *      z = 3 -> large objects like trees
         *      z = 4 -> highest layer, always visible, desciptions and such
         */

        Item {
            id: worldBackground
            width: Game.world.map.width * cellSize
            height: Game.world.map.height * cellSize
            z: Map.Layer0Lowest

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

            Rectangle {
                id: boundingRectangle
                color: "white"
                border.color: "red"
                border.width: 3
                antialiasing: true
                visible: Game.debugging
                opacity: 0.2
                width: Game.world.boundingSize.width * cellSize
                height: Game.world.boundingSize.height * cellSize
                z: Map.Layer1Lower

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
                width: root.cellSize * Game.world.player.size.width
                height: root.cellSize * Game.world.player.size.height

                antialiasing: true

                x: Game.world.player.position.x * cellSize
                y: Game.world.player.position.y * cellSize
                z: Map.Layer2Normal

                onXChanged: evaluateBoundingRectangle()
                onYChanged: evaluateBoundingRectangle()

                Component.onCompleted: evaluateBoundingRectangle()
            }

            Item {
                id: auraItem
                width: Game.world.player.auraCircleObject.size.width * cellSize
                height: Game.world.player.auraCircleObject.size.height * cellSize
                x: Game.world.player.auraCircleObject.position.x * cellSize
                y: Game.world.player.auraCircleObject.position.y * cellSize
                z: Game.world.player.auraCircleObject.layer

                opacity: Game.debugging ? 0.5 : 0

                Image {
                    id: auraImage
                    anchors.fill: parent
                    source: dataDirectory + "/images/characters/player-aura.png"
                }
            }

            GameLabel {
                id: playerNameLabel
                anchors.horizontalCenter: playerItem.horizontalCenter
                anchors.bottom: playerItem.top
                anchors.bottomMargin: 4
                visible: Game.debugging
                color: "black"
                opacity: 1
                z: Map.Layer4Highest
                text: Game.world.player.name
            }

            Repeater {
                id: itemsRepeater
                model: Game.world.gameItems
                delegate: GameItem {
                    gameItem: Game.world.gameItems.get(model.index)
                    width: model.size.width * cellSize
                    height: model.size.height * cellSize
                    x: model.position.x * cellSize
                    y: model.position.y * cellSize
                    z: model.layer
                }
            }
        }
    }

    //    MouseArea {
    //        id: sceenMouseArea
    //        anchors.fill: parent
    //        hoverEnabled: true
    //        //visible: Game.world.playerController.controlMode === Game.ControlModeKeyBoardMouse
    //        preventStealing: Game.world.playerController.controlMode === Game.ControlModeKeyBoardMouse
    //        onMouseXChanged: calculateAngle()
    //        onMouseYChanged: calculateAngle()
    //        onClicked: {
    //            if (inventoryPopup.opened) {
    //                inventoryPopup.close()
    //            } else {
    //                inventoryPopup.open()
    //            }
    //        }
    //    }

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

    function calculateAngle() {
        if (!Game.running || Game.world.playerController.controlMode !== Game.ControlModeKeyBoardMouse)
            return;

        var dx = (worldFlickable.contentX + sceenMouseArea.mouseX) - Game.world.player.position.x * cellSize
        var dy = (worldFlickable.contentY + sceenMouseArea.mouseY) - Game.world.player.position.y * cellSize
        //console.log("--> ",  dy, " | ", dx)
        Game.world.player.angle = Math.atan2(dy , dx)
    }


    RowLayout {
        id: bottomLeftOptions
        anchors.left: parent.left
        anchors.leftMargin: app.margins
        anchors.bottom: parent.bottom
        anchors.bottomMargin: app.margins

        Rectangle {
            id: pauseButton
            height: 40
            width: height
            radius: height * 0.2

            Label {
                anchors.centerIn: parent
                text: "II"
            }

            MouseArea {
                anchors.fill: parent
                onClicked: menuPopup.open()
            }
        }
    }


    Popup {
        id: menuPopup
        x: root.width * 0.15
        y: root.height * 0.1
        width: root.width * 0.7
        height: root.height * 0.8
        modal: true
        focus: true

        contentItem: Item {
            ColumnLayout {
                anchors.fill: parent
                anchors.margins: app.margins

                GameLabel {
                    text: "Game paused"
                    font.pixelSize: app.largeFont
                    Layout.alignment: Qt.AlignHCenter
                }

                Button {
                    Layout.fillWidth: true
                    text: qsTr("Main menu")
                    onClicked: {
                        menuPopup.close()
                        pageStack.pop()
                    }
                }

                Button {
                    Layout.fillWidth: true
                    text: qsTr("Settings")
                    onClicked: {
                        menuPopup.close()
                        pageStack.push(Qt.resolvedUrl("SettingsPage.qml"))
                    }
                }

                Button {
                    Layout.fillWidth: true
                    text: qsTr("Close")
                    onClicked: {
                        Game.running = true
                        menuPopup.close()
                    }
                }
            }
        }

        onOpened: Game.running = false
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent
    }

    InventoryPage {
        id: inventoryItem
        anchors.fill: parent
        visible: false

        Keys.onPressed: Game.keyPressed(event.key, event.isAutoRepeat)
        Keys.onReleased: Game.keyReleased(event.key, event.isAutoRepeat)
        onVisibleChanged: visible ? Game.running = false : Game.running = true
    }

}
