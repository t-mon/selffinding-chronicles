import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.0

import Chronicles 1.0

import "components"
import "gameitems"
import "inventory"
import "gamepages"

Item {
    id: root

    property bool debugView: true
    property bool inventoryVisible: false

    Component.onCompleted: {
        console.log("Game scene size: " + root.width + "/" + root.height)
        forceActiveFocus()
    }

    Keys.onPressed: Game.keyPressed(event.key, event.isAutoRepeat)
    Keys.onReleased: Game.keyReleased(event.key, event.isAutoRepeat)

    //    Connections {
    //        target: Game
    //        onTick: {
    //            calculateAngle()
    //        }
    //        onPaintEvent: {
    //            //console.log("paint event")
    //        }
    //    }

    Connections {
        target: Game.world.player
        onPositionChanged: {
            evaluateBoundingRectangle()
        }
    }

    //    Connections {
    //        target: Game.world
    //        onCurrentConversationChanged: {
    //            if (conversation && !inventoryItem.visible) {
    //                conversationItem.visible = true
    //            } else {
    //                conversationItem.visible = false
    //            }
    //        }

    //        onCurrentChestItemChanged: {
    //            if (chestItem && !unlockingItem.visible) {
    //                unlockingItem.visible = true
    //            } else {
    //                unlockingItem.visible = false
    //            }
    //        }
    //    }

//    Connections {
//        target: Game.world.playerController
//        onInventoryPressed: {
//            if (!conversationItem.visible) {
//                inventoryItem.visible = !inventoryItem.visible
//            }
//        }
//    }

    Flickable {
        id: worldFlickable
        anchors.fill: parent
        contentWidth: worldBackground.width
        contentHeight: worldBackground.height

        enabled: false
        antialiasing: app.antialiasing

        onContentXChanged: {
            // Calculate the new screen position
            var contentOffsetX = parseInt(contentX / app.gridSize)
            var contentOffsetY = parseInt(contentY / app.gridSize)
            Game.world.currentViewOffset = Qt.point(contentOffsetX, contentOffsetY)
        }

        onContentYChanged: {
            // Calculate the new screen position
            var contentOffsetX = parseInt(contentX / app.gridSize)
            var contentOffsetY = parseInt(contentY / app.gridSize)
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
            width: Game.world.map.width * app.gridSize
            height: Game.world.map.height * app.gridSize
            z: Map.Layer0Lowest

            Repeater {
                id: fieldRepeater
                model: Game.world
                delegate: FieldItem {
                    width: app.gridSize
                    height: app.gridSize
                    field: Game.world.get(model.index)
                    x: field.position.x * app.gridSize
                    y: field.position.y * app.gridSize
                }
            }

            Rectangle {
                id: boundingRectangle
                color: "white"
                border.color: "red"
                border.width: 3
                antialiasing: app.antialiasing
                visible: Game.debugging
                opacity: 0.2
                width: Game.world.boundingSize.width * app.gridSize
                height: Game.world.boundingSize.height * app.gridSize
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

            CharacterItem {
                id: playerItem
                character: Game.world.player

                width: Game.world.player.size.width * app.gridSize
                height: Game.world.player.size.height * app.gridSize

                x: character.position.x * app.gridSize
                y: character.position.y * app.gridSize
                z: Map.Layer2Normal

                onXChanged: evaluateBoundingRectangle()
                onYChanged: evaluateBoundingRectangle()

                Component.onCompleted: evaluateBoundingRectangle()
            }

            Repeater {
                id: itemsRepeater
                model: Game.world.gameItems
                delegate: GameItem {
                    width: model.size.width * app.gridSize
                    height: model.size.height * app.gridSize
                    x: model.position.x * app.gridSize
                    y: model.position.y * app.gridSize
                    z: model.layer
                }
            }

            Repeater {
                id: characersRepeater
                model: Game.world.characterItems
                delegate: CharacterItem {
                    character: Game.world.characterItems.get(model.index)
                    width: model.size.width * app.gridSize
                    height: model.size.height * app.gridSize
                    x: model.position.x * app.gridSize
                    y: model.position.y * app.gridSize
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
        var worldWidth = Game.world.size.width * app.gridSize
        var worldHeight = Game.world.size.height * app.gridSize

        var offsetX = Game.world.player.position.x * app.gridSize - (boundingRectangle.x + boundingRectangle.width / 2)
        var offsetY = Game.world.player.position.y * app.gridSize - (boundingRectangle.y + boundingRectangle.height / 2)

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

        var dx = (worldFlickable.contentX + sceenMouseArea.mouseX) - Game.world.player.position.x * app.gridSize
        var dy = (worldFlickable.contentY + sceenMouseArea.mouseY) - Game.world.player.position.y * app.gridSize
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

    states: [
        State {
            name: "loadingState"
            when: Game.world.state === World.StateLoading
            PropertyChanges { target: loadingItem; visible: true }
            PropertyChanges { target: conversationItem; opacity: 0 }
            PropertyChanges { target: inventoryItem; opacity: 0 }
            PropertyChanges { target: unlockingItem; opacity: 0 }
            PropertyChanges { target: plunderItem; opacity: 0 }
        },
        State {
            name: "runningState"
            when: Game.world.state === World.StateRunning
            PropertyChanges { target: loadingItem; visible: false }
            PropertyChanges { target: conversationItem; opacity: 0 }
            PropertyChanges { target: inventoryItem; opacity: 0 }
            PropertyChanges { target: unlockingItem; opacity: 0 }
            PropertyChanges { target: plunderItem; opacity: 0 }
        },
        State {
            name: "pausedState"
            when: Game.world.state === World.StatePaused
            PropertyChanges { target: loadingItem; visible: false }
            PropertyChanges { target: conversationItem; opacity: 0 }
            PropertyChanges { target: inventoryItem; opacity: 0 }
            PropertyChanges { target: unlockingItem; opacity: 0 }
            PropertyChanges { target: plunderItem; opacity: 0 }
        },
        State {
            name: "inventoryState"
            when: Game.world.state === World.StateInventory
            PropertyChanges { target: loadingItem; visible: false }
            PropertyChanges { target: conversationItem; opacity: 0 }
            PropertyChanges { target: inventoryItem; opacity: 1 }
            PropertyChanges { target: unlockingItem; opacity: 0 }
            PropertyChanges { target: plunderItem; opacity: 0 }
        },
        State {
            name: "conversationState"
            when: Game.world.state === World.StateConversation
            PropertyChanges { target: loadingItem; visible: false }
            PropertyChanges { target: conversationItem; opacity: 1 }
            PropertyChanges { target: inventoryItem; opacity: 0 }
            PropertyChanges { target: unlockingItem; opacity: 0 }
            PropertyChanges { target: plunderItem; opacity: 0 }
        },
        State {
            name: "unlockingState"
            when: Game.world.state === World.StateUnlocking
            PropertyChanges { target: loadingItem; visible: false }
            PropertyChanges { target: conversationItem; opacity: 0 }
            PropertyChanges { target: inventoryItem; opacity: 0 }
            PropertyChanges { target: unlockingItem; opacity: 1 }
            PropertyChanges { target: plunderItem; opacity: 0 }
        },
        State {
            name: "tradeState"
            when: Game.world.state === World.StateTrade
            PropertyChanges { target: loadingItem; visible: false }
            PropertyChanges { target: conversationItem; opacity: 0 }
            PropertyChanges { target: inventoryItem; opacity: 0 }
            PropertyChanges { target: unlockingItem; opacity: 0 }
            PropertyChanges { target: plunderItem; opacity: 0 }
        },
        State {
            name: "plunderState"
            when: Game.world.state === World.StatePlunder
            PropertyChanges { target: loadingItem; visible: false }
            PropertyChanges { target: conversationItem; opacity: 0 }
            PropertyChanges { target: inventoryItem; opacity: 0 }
            PropertyChanges { target: unlockingItem; opacity: 0 }
            PropertyChanges { target: plunderItem; opacity: 1 }
        }
    ]


    InventoryPage {
        id: inventoryItem
        anchors.fill: parent
        opacity: 0

        Keys.onPressed: Game.keyPressed(event.key, event.isAutoRepeat)
        Keys.onReleased: Game.keyReleased(event.key, event.isAutoRepeat)
        //onVisibleChanged: visible ? Game.running = false : Game.running = true
    }

    ConversationPage {
        id: conversationItem
        anchors.fill: parent
        opacity: 0

        Keys.onPressed: Game.keyPressed(event.key, event.isAutoRepeat)
        Keys.onReleased: Game.keyReleased(event.key, event.isAutoRepeat)
        //onVisibleChanged: visible ? Game.running = false : Game.running = true
    }

    UnlockingPage {
        id: unlockingItem
        anchors.fill: parent
        opacity: 0

        Keys.onPressed: Game.keyPressed(event.key, event.isAutoRepeat)
        Keys.onReleased: Game.keyReleased(event.key, event.isAutoRepeat)
        //onVisibleChanged: visible ? Game.running = false : Game.running = true
    }

    PlunderPage {
        id: plunderItem
        anchors.fill: parent
        opacity: 0

        Keys.onPressed: Game.keyPressed(event.key, event.isAutoRepeat)
        Keys.onReleased: Game.keyReleased(event.key, event.isAutoRepeat)
        //onVisibleChanged: visible ? Game.running = false : Game.running = true
    }


    Rectangle {
        id: loadingItem
        anchors.fill: parent
        color: app.backgroundColor
        visible: false

        Column {
            id: loadingColumn
            anchors.centerIn: parent

            GameLabel {
                id: loadingLabel
                anchors.horizontalCenter: parent.horizontalCenter
                text: "Loading..."
                font.pixelSize: app.largeFont
                color: "white"
            }

            BusyIndicator {
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }
    }

}
