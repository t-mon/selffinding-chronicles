import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.0
import QtQuick.Controls.Material 2.0
import Qt.labs.folderlistmodel 2.1

import Box2D 2.0
import Chronicles 1.0

import "../components"
import "../"
import "../gameitems"
import "../gameoverlays"
import "../physics"

GamePage {
    id: root

    title: qsTr("Map editor")

    header: ToolBar {
        background: Rectangle {
            implicitHeight: 40
            color: Material.color(Material.BlueGrey)
        }

        RowLayout {
            anchors.fill: parent
            HeaderButton {
                enabled: !workingOverlay.visible
                imageSource: dataDirectory + "/icons/back.svg"
                onClicked: pageStack.pop()
            }

            Label {
                text: Game.mapEditor.map.name
            }

            Item {
                Layout.fillHeight: true
                Layout.fillWidth: true
            }

            HeaderButton {
                enabled: !workingOverlay.visible
                imageSource: dataDirectory + "/icons/settings.svg"
                onClicked: console.log("Open map settings")
            }
        }
    }

    Component.onCompleted: {
        console.log("Game scene size:", root.width, "/", root.height, "grid size:", app.gridSize)
        Game.mapEditor.loadAvailableGameItems()
        Game.mapEditor.loadAvailableObjects()
        Game.mapEditor.loadAvailableCharacters()
        Game.mapEditor.loadAvailableEnemies()
    }

    property string currentResourcePath: ""

    // Selection mode
    property var selectedItem: null
    onSelectedItemChanged: {
        if (selectedItem) {
            console.log("--> Selected item", selectedItem.name, selectedItem.position.x, selectedItem.position.y)
        }
    }

    function selectItem(item) {
        if (item && selectedItem !== item) {
            //console.log("--> Selected item", item.name, item.position.x, item.position.y)
            if (selectedItem)
                selectedItem.playerFocus = false

            selectedItem = item
            item.playerFocus = true
        } else if (selectedItem && !item) {
            console.log("--> No item selected")
            selectedItem.playerFocus = false
            selectedItem = null
        }
    }


    Connections {
        target: Game.mapEditor
        function onToolChanged(tool) {
            switch (tool) {
            case GameMapEditor.ToolSelect:
                selectedItem = null
                break;
            case GameMapEditor.ToolPlace:
                selectedItem = null
                break;
            case GameMapEditor.ToolMove:
                selectedItem = null
                break;
            }
        }
    }

    // Pysical world
    World {
        id: physicsWorld
        gravity: Qt.point(0, 0)
        pixelsPerMeter: app.gridSize
        //onStepped: Game.onTick()
        running: true
    }


    ColumnLayout {
        id: mainColumn
        anchors.fill: parent

        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true

            RowLayout {
                id: mainRow
                anchors.fill: parent

                Item {
                    id: mainControlsItem
                    Layout.fillHeight: true
                    Layout.preferredWidth: 300
                    z: 1

                    Loader {
                        anchors.fill: parent
                        sourceComponent: Game.mapEditor.tool == GameMapEditor.ToolPlace ? availableItemsComponent : colorPickerComponent
                    }

                    Component {
                        id: colorPickerComponent
                        ColorPicker {
                            id: colorPicker
                        }
                    }

                    Component {
                        id: availableItemsComponent
                        AvailableItemMenu {
                            id: mainControlsColumn
                        }
                    }
                }

                GameScene {
                    id: gameScene
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    opacity: 1
                    physicsWorld: physicsWorld
                    mapScene: Game.mapEditor.mapScene
                    itemDebugEnabled: Game.settings.itemDebugEnabled
                    physicsDebugEnabled: Game.settings.physicsDebugEnabled
                    scrollBarsEnabled: true

                    Component.onCompleted: {
                        forceActiveFocus()
                        Game.mapEditor.mapScene.viewWindow = Qt.rect(gameScene.flickable.contentX / app.gridSize, gameScene.flickable.contentY / app.gridSize, gameScene.flickable.width / app.gridSize, gameScene.flickable.height / app.gridSize)
                    }

                    Connections {
                        target: gameScene.flickable
                        function onContentXChanged(contentX) {
                            updatePositions()
                            // Update the map editor view window for evaluating the active items
                            Game.mapEditor.mapScene.viewWindow = Qt.rect(gameScene.flickable.contentX / app.gridSize, gameScene.flickable.contentY / app.gridSize, gameScene.flickable.width / app.gridSize, gameScene.flickable.height / app.gridSize)
                        }

                        function onContentYChanged(contentY) {
                            updatePositions()
                            // Update the map editor view window for evaluating the active items
                            Game.mapEditor.mapScene.viewWindow = Qt.rect(gameScene.flickable.contentX / app.gridSize, gameScene.flickable.contentY / app.gridSize, gameScene.flickable.width / app.gridSize, gameScene.flickable.height / app.gridSize)
                        }
                    }

                    GameItem {
                        id: temporaryItem
                        gameItem: Game.mapEditor.selectedGameItem
                        itemDebugEnabled: Game.settings.itemDebugEnabled
                        visible: editorMouseArea.containsMouse && Game.mapEditor.tool == GameMapEditor.ToolPlace
                        width: gameItem ? gameItem.size.width * app.gridSize : 0
                        height: gameItem ? gameItem.size.height * app.gridSize : 0
                    }

                    MouseArea {
                        id: editorMouseArea
                        anchors.fill: parent
                        //enabled: Game.mapEditor.tool !== GameMapEditor.ToolMove
                        hoverEnabled: true
                        preventStealing: true

                        // Move tool drag properties
                        property bool moveDrag: false
                        property point moveDragPoint: Qt.point(0,0)
                        onMoveDragChanged: console.log("Move drag", moveDrag)
                        acceptedButtons: Qt.LeftButton | Qt.RightButton | Qt.MiddleButton
                        onMouseXChanged: {
                            //console.log("--> mouse position changed", mouseX, mouseY)

                            switch (Game.mapEditor.tool) {
                            case GameMapEditor.ToolPlace:
                                updatePositions()
                                break;
                            case GameMapEditor.ToolSelect:
                                selectItemUnderMouse()
                                break;
                            case GameMapEditor.ToolMove:
                                if (moveDrag) {
                                    gameScene.flickable.contentX += moveDragPoint.x - mouseX
                                    gameScene.flickable.contentY += moveDragPoint.y - mouseY
                                    moveDragPoint = Qt.point(mouseX, mouseY)
                                }
                                break;
                            }
                        }

                        onMouseYChanged: {
                            //console.log("--> mouse position changed", mouseX, mouseY)

                            switch (Game.mapEditor.tool) {
                            case GameMapEditor.ToolPlace:
                                updatePositions()
                                break;
                            case GameMapEditor.ToolSelect:
                                selectItemUnderMouse()
                                break;
                            case GameMapEditor.ToolMove:
                                if (moveDrag) {
                                    gameScene.flickable.contentX += moveDragPoint.x - mouseX
                                    gameScene.flickable.contentY += moveDragPoint.y - mouseY
                                    moveDragPoint = Qt.point(mouseX, mouseY)
                                }
                                break;
                            }
                        }

                        onClicked: {
                            if (mouse.button == Qt.RightButton) {
                                // TODO: If item selected -> item menu
                                console.log("Open map menu")
                                mapMenu.x = mouse.x
                                mapMenu.y = mouse.y
                                mapMenu.open()
                                return
                            }


                            switch (Game.mapEditor.tool) {
                            case GameMapEditor.ToolPlace:
                                if (temporaryItem) {
                                    var position = Qt.point((gameScene.flickable.contentX + temporaryItem.x) / app.gridSize,
                                                            (gameScene.flickable.contentY + temporaryItem.y) / app.gridSize)
                                    Game.mapEditor.placeItemOnMap(temporaryItem.gameItem.resourcePath, position)
                                }
                                break;
                            case GameMapEditor.ToolSelect:

                                break;
                            case GameMapEditor.ToolMove:

                                break;
                            }
                        }

                        onPressed: {
                            switch (Game.mapEditor.tool) {
                            case GameMapEditor.ToolPlace:
                                break;
                            case GameMapEditor.ToolSelect:
                                break;
                            case GameMapEditor.ToolMove:
                                moveDragPoint = Qt.point(mouseX, mouseY)
                                moveDrag = true
                                break;
                            }
                        }

                        onReleased: {
                            switch (Game.mapEditor.tool) {
                            case GameMapEditor.ToolPlace:
                                break;
                            case GameMapEditor.ToolSelect:
                                break;
                            case GameMapEditor.ToolMove:
                                moveDrag = false
                                break;
                            }
                        }
                    }

                    Menu {
                        id: mapMenu
                        title: "Map menu"
                        MenuItem {
                            text: "World color"
                        }

                        MenuItem {
                            text: "Test..."
                        }
                    }
                }
            }
        }

        MapEditorToolBar {
            id: toolBar
            Layout.fillWidth: true
            Layout.preferredHeight: 40
        }
    }

    Item {
        id: workingOverlay
        anchors.fill: parent
        visible: Game.mapEditor.dataManager.state != DataManager.StateIdle

        Rectangle {
            id: loadingScreen
            anchors.fill: parent
            color: app.backgroundColor

            Column {
                id: loadingColumn
                anchors.centerIn: parent

                GameLabel {
                    id: loadingLabel
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: {
                        switch (Game.mapEditor.dataManager.state) {
                        case DataManager.StateLoading:
                            return qsTr("Loading...")
                        case DataManager.StateSaving:
                            return qsTr("Saving...")
                        default:
                            return ""
                        }
                    }
                    font.pixelSize: app.largeFont
                    color: "white"
                }

                GameBusyIndicator {
                    anchors.horizontalCenter: parent.horizontalCenter
                    running: true
                }
            }
        }
    }


    function mouseItemIntercepts(item, positionX, positionY) {
        var itemRectangle = Qt.rect(item.gameItem.position.x * app.gridSize, item.gameItem.position.y * app.gridSize, item.gameItem.size.width * app.gridSize, item.gameItem.size.height * app.gridSize)
        //console.log("Checking position", Qt.point(positionX, positionY), item.name, itemRectangle)
        if (positionX >= itemRectangle.x && positionX <= itemRectangle.x + itemRectangle.width &&
                positionY >= itemRectangle.y && positionY < itemRectangle.y + itemRectangle.height) {
            return true;
        }

        return false;
    }

    // Returns the center point of the given item in screen coordinates
    function getItemCenterScreenCoordinates(item) {
        var itemCenterScreenPositionX = item.gameItem.position.x * app.gridSize + item.gameItem.size.width * app.gridSize / 2 - gameScene.flickable.contentX
        var itemCenterScreenPositionY = item.gameItem.position.y * app.gridSize + item.gameItem.size.height * app.gridSize / 2 - gameScene.flickable.contentY
        return Qt.point(itemCenterScreenPositionX, itemCenterScreenPositionY)
    }

    function updatePositions() {
        // TODO: update position for view

        if (!temporaryItem)
            return

        var positionX = editorMouseArea.mouseX - temporaryItem.width / 2
        var positionY = editorMouseArea.mouseY - temporaryItem.height / 2

        if (Game.settings.gridSnapping) {
            positionX = Math.round(positionX / app.gridSize) * app.gridSize
            positionY = Math.round(positionY / app.gridSize) * app.gridSize
        }

        temporaryItem.x = positionX
        temporaryItem.y = positionY
    }

    function selectItemUnderMouse() {
        var positionX = gameScene.flickable.contentX + editorMouseArea.mouseX
        var positionY = gameScene.flickable.contentY + editorMouseArea.mouseY

        // Select object or item, depending on the mode
        if (Game.mapEditor.mode == GameMapEditor.ModeObjects) {
            var objectsUnderMouse = []
            // TODO
        } else {
            // The rest are game items
            var itemsUnderMouse = []

            switch (Game.mapEditor.mode) {
            case GameMapEditor.ModeItems:
                for (var itemsIndex = 0; itemsIndex < gameScene.items.count  ; itemsIndex++) {
                    var item = gameScene.items.itemAt(itemsIndex)
                    if (mouseItemIntercepts(item, positionX, positionY)) {
                        itemsUnderMouse.push(item)
                    }
                }
                break;
            default:
                console.warn("Unhandled mode for selecting item/object")
                break;
            }

            // Now check all intercepting items if the pixel under the mouse is 0
            if (itemsUnderMouse.length > 0) {
                //console.log("Intercepting", itemsUnderMouse.length, "item areas")
                // For now, get the distance to the center for selecting
                var closestItem = null
                var bestCenterDistance = -1
                for (var x = 0; x < itemsUnderMouse.length; x++) {
                    // Calculate the distance to the center
                    var interceptingItem = itemsUnderMouse[x]

                    // Calculate the item center screen position
                    var itemCenterScreenPosition = getItemCenterScreenCoordinates(interceptingItem)
                    var distance = Math.sqrt(Math.pow(itemCenterScreenPosition.x - editorMouseArea.mouseX, 2) + Math.pow(itemCenterScreenPosition.y - editorMouseArea.mouseY, 2))
                    //console.log("Center distance from",interceptingItem.gameItem.name, distance)
                    if (bestCenterDistance < 0) {
                        closestItem = interceptingItem
                        bestCenterDistance = distance
                    } else if (distance < bestCenterDistance) {
                        closestItem = interceptingItem
                        bestCenterDistance = distance
                    }
                }
                selectItem(closestItem.gameItem)
            } else {
                selectItem(null)
            }
        }

        //
    }
}

