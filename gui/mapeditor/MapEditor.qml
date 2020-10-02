import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.0
import QtQuick.Controls.Material 2.0
import Qt.labs.folderlistmodel 2.1

import Box2D 2.0
import Chronicles 1.0

import "components"
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
        }
    }

    // Shortcuts

    Keys.onPressed: {
        switch (event.key) {
        case Qt.Key_S:
            Game.mapEditor.tool = GameMapEditor.ToolSelect
            break;
        case Qt.Key_M:
            Game.mapEditor.tool = GameMapEditor.ToolMove
            break;
        case Qt.Key_P:
            Game.mapEditor.tool = GameMapEditor.ToolPlace
            break;
        case Qt.Key_G:
            Game.settings.gridSnapping = !Game.settings.gridSnapping
            break;
        case Qt.Key_Backspace:
            if (currentSelectedItem) {
                Game.mapEditor.removeGameItem(currentSelectedItem.gameItem)
            }
            break;
        }
    }


    Component.onCompleted: {
        console.log("Game scene size:", root.width, "/", root.height, "grid size:", app.gridSize)
        Game.mapEditor.loadAvailableGameItems()
        Game.mapEditor.loadAvailableObjects()
        Game.mapEditor.loadAvailableCharacters()
        Game.mapEditor.loadAvailableEnemies()
    }

    // Selection mode
    property var currentItemUnderCurser: null
    property var currentSelectedItem: null

    Connections {
        target: Game.mapEditor
        function onToolChanged(tool) {
            switch (tool) {
            case GameMapEditor.ToolSelect:
                currentItemUnderCurser = null
                currentSelectedItem = null
                evaluateItemUnderCurser()
                break;
            case GameMapEditor.ToolPlace:
                currentItemUnderCurser = null
                currentSelectedItem = null
                updatePositions()
                break;
            case GameMapEditor.ToolMove:
                currentItemUnderCurser = null
                currentSelectedItem = null
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

                        Item {
                            ColumnLayout {
                                anchors.fill: parent


                                GameLabel {
                                    Layout.fillWidth: true
                                    Layout.preferredHeight: 30
                                    verticalAlignment: Qt.AlignVCenter
                                    color: "white"
                                    text: qsTr("World background color")
                                }

                                ColorPicker {
                                    Layout.fillWidth: true
                                    id: colorPicker
                                    function onColorChanged(color) {
                                        gameScene.mapScene.map.backgroundColor = color
                                    }

                                    Component.onCompleted: currentColor = gameScene.mapScene.map.backgroundColor
                                }

                                Item { Layout.fillWidth: true; Layout.fillHeight: true }
                            }
                        }
                    }

                    Component {
                        id: availableItemsComponent
                        AvailableItemMenu {
                            id: mainControlsColumn
                        }
                    }
                }

                // ##################################################################################
                // Game scene
                // ##################################################################################

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

                    Loader {
                        id: temporaryItemLoader
                        active: Game.mapEditor.selectedGameItem && editorMouseArea.containsMouse && Game.mapEditor.tool == GameMapEditor.ToolPlace
                        sourceComponent: temporaryItemComponent

                        Component{
                            id: temporaryItemComponent
                            Item {
                                id: temporaryItem
                                property bool itemDebugEnabled: Game.settings.itemDebugEnabled

                                width: Game.mapEditor.selectedGameItem.size.width * app.gridSize
                                height: Game.mapEditor.selectedGameItem.size.height * app.gridSize

                                Image {
                                    id: itemImage
                                    anchors.fill: parent
                                    source: dataDirectory + Game.mapEditor.selectedGameItem.imageName
                                    opacity: Game.settings.itemDebugEnabled ? 0.5 : 1
                                }

                                ItemDescription {
                                    id: nameLabel
                                    anchors.bottom: temporaryItem.top
                                    anchors.horizontalCenter: temporaryItem.horizontalCenter
                                    text: Game.mapEditor.selectedGameItem.name
                                }

                                Loader {
                                    anchors.fill: parent
                                    active: Game.settings.itemDebugEnabled
                                    source: "../components/ItemDebugFrame.qml"
                                }
                            }
                        }
                    }

                    // ##################################################################################
                    // Editor mouse area
                    // ##################################################################################

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
                                evaluateItemUnderCurser()
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
                                evaluateItemUnderCurser()
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
                                if (temporaryItemLoader.active) {
                                    var position = Qt.point((gameScene.flickable.contentX + temporaryItemLoader.item.x) / app.gridSize,
                                                            (gameScene.flickable.contentY + temporaryItemLoader.item.y) / app.gridSize)
                                    Game.mapEditor.placeItemOnMap(Game.mapEditor.selectedGameItem.resourcePath, position)
                                }
                                break;
                            case GameMapEditor.ToolSelect:
                                if (root.currentItemUnderCurser) {
                                    setCurrentSelectedItem(root.currentItemUnderCurser)
                                } else {
                                    setCurrentSelectedItem(null)
                                    evaluateItemUnderCurser()
                                }

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


                    Loader {
                        id: selectedItemRegionLoader
                        //anchors.fill: parent
                        active: root.currentSelectedItem !== null
                        sourceComponent: itemRegionComponent

                        Component {
                            id: itemRegionComponent

                            Rectangle {
                                id: selectedItemRegion
                                x: root.currentSelectedItem.x - gameScene.flickable.contentX
                                y: root.currentSelectedItem.y - gameScene.flickable.contentY
                                height: root.currentSelectedItem.height
                                width: root.currentSelectedItem.width
                                color: "#354682B4"
                                border {
                                    width: 2
                                    color: "steelblue"
                                }

                                property bool moveItemDrag: false
                                property point moveItemDragPoint: Qt.point(0,0)
                                onMoveItemDragChanged: console.log("Move item", root.currentSelectedItem.gameItem.name)

                                MouseArea {
                                    anchors.fill: parent
                                    acceptedButtons: Qt.LeftButton | Qt.RightButton | Qt.MiddleButton
                                    onClicked: {
                                        if (mouse.button == Qt.LeftButton) {
                                            console.log("Clicked on selected item")
                                        } else if (mouse.button == Qt.RightButton) {
                                            console.log("Right clicked on selected item")
                                            console.log("Open map menu")
                                            itemMenu.x = mouse.x
                                            itemMenu.y = mouse.y
                                            itemMenu.open()
                                        }
                                    }

                                    //                                    function evaluateMouseChanged() {
                                    //                                        if (moveItemDrag) {
                                    //                                            var deltaX = mouseX - moveItemDragPoint.x
                                    //                                            var deltaY = mouseY - moveItemDragPoint.y

                                    //                                            console.log("Raw delta", deltaX, deltaY)

                                    //                                            if (Game.settings.gridSnapping) {

                                    //                                            } else {
                                    //                                                root.currentSelectedItem.gameItem.position.x += (deltaX / app.gridSize)
                                    //                                                root.currentSelectedItem.gameItem.position.y += (deltaY / app.gridSize)
                                    //                                                moveItemDragPoint = Qt.point(mouseX, mouseY)
                                    //                                            }

                                    //                                            if (Game.settings.gridSnapping) {
                                    //                                                deltaX = Math.round(deltaX / app.gridSize) * app.gridSize
                                    //                                                deltaY = Math.round(deltaY / app.gridSize) * app.gridSize
                                    //                                            }

                                    //                                            console.log("Actual delta", deltaX, deltaY)

                                    //                                            var positionX = (root.currentSelectedItem.x - gameScene.flickable.contentX + deltaX) / app.gridSize
                                    //                                            var positionY = (root.currentSelectedItem.y - gameScene.flickable.contentY + deltaY) / app.gridSize

                                    //                                            console.log("Absolute position", positionX, positionY)
                                    //                                            root.currentSelectedItem.gameItem.position.x = positionX
                                    //                                            root.currentSelectedItem.gameItem.position.y = positionY
                                    //                                            moveItemDragPoint = Qt.point(mouseX, mouseY)
                                    //                                        }
                                    //                                    }

                                    onMouseXChanged: {
                                        if (moveItemDrag) {
                                            var deltaX = mouseX - moveItemDragPoint.x
                                            console.log("Raw delta x", deltaX)

                                            if (Game.settings.gridSnapping) {

                                            } else {
                                                moveItemDragPoint.x += deltaX
                                                root.currentSelectedItem.gameItem.position.x += (deltaX / app.gridSize)
                                            }
                                        }
                                    }

                                    onMouseYChanged: {
                                        if (moveItemDrag) {
                                            var deltaY = mouseY - moveItemDragPoint.y
                                            console.log("Raw delta y", deltaY)

                                            if (Game.settings.gridSnapping) {

                                            } else {
                                                moveItemDragPoint.y += deltaY
                                                root.currentSelectedItem.gameItem.position.y += (deltaY / app.gridSize)
                                            }
                                        }
                                    }

                                    onPressed: {
                                        moveItemDragPoint = Qt.point(mouseX, mouseY)
                                        console.log("Start draging", moveItemDragPoint.x, moveItemDragPoint.y)
                                        moveItemDrag = true
                                    }

                                    onReleased: {
                                        moveItemDrag = false
                                    }
                                }
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

                        MenuItem {
                            text: "Add light source"
                            onTriggered: {
                                Game.mapEditor.addBackgroundLightSource(LightSource.LightTypeSpotlight, "#FFFFFF", convertMouseCoordinatesToWorldCoordinates(mapMenu.x, mapMenu.y))
                            }
                        }
                    }

                    Menu {
                        id: itemMenu
                        title: "Item menu"
                        MenuItem {
                            text: "Delete"
                        }

                        MenuItem {
                            text: "Rotates"
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

    // ##################################################################################
    // Functions
    // ##################################################################################

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

    // ##################################################################################
    // Functions
    // ##################################################################################

    function setCurrentItemUnderCurser(item) {
        if (currentSelectedItem) {
            return
        }

        if (item && currentItemUnderCurser !== item) {
            if (currentItemUnderCurser)
                currentItemUnderCurser.gameItem.playerFocus = false

            currentItemUnderCurser = item
            console.log("--> Current item under curser:", currentItemUnderCurser.gameItem.name, currentItemUnderCurser.gameItem.position.x, currentItemUnderCurser.gameItem.position.y)
            item.gameItem.playerFocus = true
        } else if (currentItemUnderCurser && !item) {
            console.log("--> No item under curser")
            currentItemUnderCurser.gameItem.playerFocus = false
            currentItemUnderCurser = null
        }
    }


    function setCurrentSelectedItem(item) {
        if (item && currentSelectedItem !== item) {
            currentItemUnderCurser = null
            currentSelectedItem = item
            currentSelectedItem.gameItem.playerFocus = true
            console.log("--> Select item:", currentSelectedItem.gameItem.name, currentSelectedItem.gameItem.position.x, currentSelectedItem.gameItem.position.y)
        } else if (currentSelectedItem && !item) {
            currentItemUnderCurser = null
            currentSelectedItem.gameItem.playerFocus = false
            currentSelectedItem = item
            console.log("--> No item selected")
        }
    }


    function mouseInterceptsItem(item, positionX, positionY) {
        var itemRectangle = Qt.rect(item.gameItem.position.x * app.gridSize, item.gameItem.position.y * app.gridSize, item.gameItem.size.width * app.gridSize, item.gameItem.size.height * app.gridSize)
        //console.log("Checking position", Qt.point(positionX, positionY), item.name, itemRectangle)
        if (positionX >= itemRectangle.x &&
                positionX <= itemRectangle.x + itemRectangle.width &&
                positionY >= itemRectangle.y &&
                positionY < itemRectangle.y + itemRectangle.height) {
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

    // Returns the given mouse coordinates in world coordinates
    function convertMouseCoordinatesToWorldCoordinates(mouseX, mouseY) {
        var worldX = (mouseX + gameScene.flickable.contentX) / app.gridSize
        var worldY = (mouseY + gameScene.flickable.contentY) / app.gridSize
        console.log("Mouse position", mouseX, mouseY, "--> world position", worldX, worldY)
        return Qt.point(worldX, worldY)
    }

    function updatePositions() {
        // TODO: update position for view

        if (!temporaryItemLoader.active)
            return

        var positionX = editorMouseArea.mouseX - temporaryItemLoader.item.width / 2
        var positionY = editorMouseArea.mouseY - temporaryItemLoader.item.height / 2

        if (Game.settings.gridSnapping) {
            positionX = Math.round(positionX / app.gridSize) * app.gridSize
            positionY = Math.round(positionY / app.gridSize) * app.gridSize
        }

        temporaryItemLoader.item.x = positionX
        temporaryItemLoader.item.y = positionY
    }

    function evaluateItemUnderCurser() {
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
                    if (mouseInterceptsItem(item, positionX, positionY)) {
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
                // For now, get the shortest distance to the center for selecting
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
                setCurrentItemUnderCurser(closestItem)
            } else {
                setCurrentItemUnderCurser(null)
            }
        }
    }
}

