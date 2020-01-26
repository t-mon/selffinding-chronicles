import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.0
import QtQuick.Controls.Material 2.0
import Qt.labs.folderlistmodel 2.1

import Box2D 2.0
import Chronicles 1.0

import "../components"
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
        forceActiveFocus()
        Game.mapEditor.loadAvailableGameItems()
        Game.mapEditor.loadAvailableObjects()
        Game.mapEditor.loadAvailableCharacters()
        Game.mapEditor.loadAvailableEnemies()
        Game.mapEditor.onEditorViewSizeChanged(Qt.size(root.width, root.height))
    }

    property string currentResourcePath: ""

    //property var currentItem: Game.mapEditor.availableItems.get(itemsListView.currentIndex)


    // Selection mode
    property var selectedItem: null
    onSelectedItemChanged: {
        if (selectedItem) {
            //console.log("--> Selected item", selectedItem.name)
        }
    }

    function selectItem(item) {
        if (item && selectedItem !== item) {
            console.log("--> Selected item", item.name)
            selectedItem = item
        } else if (selectedItem && !item) {
            console.log("--> No item selected")
            selectedItem = null
        }
    }

    // Pysical world
    World {
        id: physicsWorld
        gravity: Qt.point(0, 0)
        //onStepped: Game.onTick()
        running: true
    }

    Connections {
        target: Game.mapEditor
        onToolChanged: {
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

    Item {
        id: sceneItem
        anchors.fill: parent

        ColumnLayout {
            anchors.fill: parent

            RowLayout {
                id: mainLayout
                Layout.fillWidth: true
                Layout.fillHeight: true

                Item {
                    id: mainControlsItem
                    Layout.fillHeight: true
                    Layout.preferredWidth: 300
                    z: 1

                    ColumnLayout {
                        id: mainControlsColumn
                        anchors.fill: parent

                        TabBar {
                            id: optionsTabBar
                            Layout.fillWidth: true
                            TabButton {
                                text: qsTr("Items")
                            }

                            TabButton {
                                text: qsTr("Objects")
                                onClicked: {

                                }

                            }

                            TabButton { text: qsTr("Characters") }
                            TabButton { text: qsTr("Enemies") }
                        }

                        StackLayout {
                            id: mainControlsLayout
                            width: parent.width
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            currentIndex: optionsTabBar.currentIndex

                            Item {
                                id: availableItems
                                Layout.fillWidth: true
                                Layout.fillHeight: true

                                ListView {
                                    id: itemsListView
                                    anchors.fill: parent
                                    clip: true
                                    ScrollBar.vertical: ScrollBar { }

                                    model: Game.mapEditor.availableItems
                                    delegate: ItemDelegate {
                                        width: parent.width
                                        highlighted: ListView.isCurrentItem
                                        onClicked: itemsListView.currentIndex = index

                                        RowLayout {
                                            anchors.fill: parent
                                            anchors.leftMargin: app.margins
                                            anchors.verticalCenter: parent.verticalCenter

                                            Image {
                                                Layout.preferredWidth: parent.height * 0.8
                                                Layout.preferredHeight: parent.height * 0.8
                                                Layout.alignment: Qt.AlignVCenter
                                                source: dataDirectory + model.imageName
                                            }

                                            Label {
                                                Layout.fillWidth: true
                                                Layout.alignment: Qt.AlignVCenter
                                                text: model.name
                                                color: itemsListView.currentIndex === index ? "black" : "white"
                                            }
                                        }
                                    }

                                    onCurrentIndexChanged: Game.mapEditor.createSelectedGameItem(Game.mapEditor.availableItems.get(itemsListView.currentIndex).resourcePath)
                                }
                            }

                            Item {
                                id: objectsItems
                                Layout.fillWidth: true
                                Layout.fillHeight: true

                                ListView {
                                    id: objectsListView
                                    anchors.fill: parent
                                    clip: true
                                    ScrollBar.vertical: ScrollBar { }

                                    model: Game.mapEditor.availableObjects
                                    delegate: ItemDelegate {
                                        width: parent.width
                                        highlighted: ListView.isCurrentItem
                                        onClicked: objectsListView.currentIndex = index

                                        RowLayout {
                                            anchors.fill: parent
                                            anchors.leftMargin: app.margins
                                            anchors.verticalCenter: parent.verticalCenter

                                            Image {
                                                Layout.preferredWidth: parent.height * 0.8
                                                Layout.preferredHeight: parent.height * 0.8
                                                Layout.alignment: Qt.AlignVCenter
                                                source: dataDirectory + model.imageName
                                            }

                                            Label {
                                                Layout.fillWidth: true
                                                Layout.alignment: Qt.AlignVCenter
                                                text: model.name
                                                color: objectsListView.currentIndex === index ? "black" : "white"
                                            }
                                        }
                                    }

                                    //onCurrentIndexChanged: Game.mapEditor.createSelectedGameObject(Game.mapEditor.availableItems.get(itemsListView.currentIndex).resourcePath)
                                }
                            }


                            Item {
                                id: availableCharacters
                                Layout.fillWidth: true
                                Layout.fillHeight: true

                                ListView {
                                    id: charactersListView
                                    anchors.fill: parent
                                    clip: true
                                    ScrollBar.vertical: ScrollBar { }

                                    model: Game.mapEditor.availableCharacters
                                    delegate: ItemDelegate {
                                        width: parent.width
                                        highlighted: ListView.isCurrentItem
                                        onClicked: charactersListView.currentIndex = index

                                        RowLayout {
                                            anchors.fill: parent
                                            anchors.leftMargin: app.margins
                                            anchors.verticalCenter: parent.verticalCenter

                                            Image {
                                                Layout.preferredWidth: parent.height * 0.8
                                                Layout.preferredHeight: parent.height * 0.8
                                                Layout.alignment: Qt.AlignVCenter
                                                source: dataDirectory + model.imageName
                                            }

                                            Label {
                                                Layout.fillWidth: true
                                                Layout.alignment: Qt.AlignVCenter
                                                text: model.name
                                                color: charactersListView.currentIndex === index ? "black" : "white"
                                            }
                                        }
                                    }

                                    onCurrentIndexChanged: Game.mapEditor.createSelectedGameItem(Game.mapEditor.availableCharacters.get(charactersListView.currentIndex).resourcePath)
                                }
                            }
                        }
                    }
                }


                // World flickble
                Item {
                    id: mainItem
                    Layout.fillHeight: true
                    Layout.fillWidth: true

                    function selectItemUnderMouse() {
                        var positionX = worldFlickable.contentX + editorMouseArea.mouseX
                        var positionY = worldFlickable.contentY + editorMouseArea.mouseY

                        var currentSelectedItem = null

                        for (var i = 0; i < Game.mapEditor.activeItems.count; i++) {
                            var item = Game.mapEditor.activeItems.get(i)
                            var itemRectangle = Qt.rect(item.position.x * app.gridSize, item.position.y * app.gridSize, item.size.width * app.gridSize, item.size.height * app.gridSize)
                            //console.log("Checking position", Qt.point(positionX, positionY), item.name, itemRectangle)
                            if (positionX >= itemRectangle.x && positionX <= itemRectangle.x + itemRectangle.width &&
                                    positionY >= itemRectangle.y && positionY < itemRectangle.y + itemRectangle.height) {

                                //console.log("Intercepts item", item.name, itemRectangle)
                                currentSelectedItem = item
                            }
                        }

                        selectItem(currentSelectedItem)
                    }

                    function updatePositions() {
                        // TODO: update position for view

                        if (!temporaryItem)
                            return

                        var positionX = worldFlickable.contentX + editorMouseArea.mouseX - temporaryItem.width / 2
                        var positionY = worldFlickable.contentY + editorMouseArea.mouseY - temporaryItem.height / 2

                        if (Game.settings.gridSnapping) {
                            positionX = Math.round(positionX / app.gridSize) * app.gridSize
                            positionY = Math.round(positionY / app.gridSize) * app.gridSize
                        }

                        temporaryItem.x = positionX
                        temporaryItem.y = positionY
                    }

                    Flickable {
                        id: worldFlickable
                        anchors.fill: parent
                        contentWidth: worldItem.width
                        contentHeight: worldItem.height
                        clip: true

                        ScrollBar.vertical: ScrollBar { active: true; interactive: true; policy: ScrollBar.AlwaysOn }
                        ScrollBar.horizontal: ScrollBar { active: true; interactive: true; policy: ScrollBar.AlwaysOn }

                        onContentXChanged: mainItem.updatePositions()
                        onContentYChanged: mainItem.updatePositions()

                        Item {
                            id: worldItem
                            width: Game.mapEditor.map.size.width * app.gridSize
                            height: Game.mapEditor.map.size.height * app.gridSize

                            Rectangle {
                                anchors.fill: parent
                                color: "#307a78"
                            }

                            WorldBoundaries {
                                id: worldBoundaries
                                worldObject: worldItem
                            }

                            function calculateLayerValue(layer, itemY, itemHeight, worldHeight) {
                                if (layer === GameObject.LayerBackground) {
                                    return -2
                                } else if (layer === GameObject.LayerBase) {
                                    return -1
                                } else if (layer === GameObject.LayerItem) {
                                    return itemY + itemHeight
                                }  else if (layer === GameObject.LayerOverlay) {
                                    return worldHeight + itemHeight + 1
                                } else {
                                    return itemY + itemHeight
                                }
                            }

                            Repeater {
                                id: gameObjectRepeater
                                model: Game.mapEditor.activeObjects
                                delegate: GameObject {
                                    id: gameObjet
                                    gameObject: Game.mapEditor.activeObjects.get(model.index)
                                    itemDebugEnabled: debugControls.itemDebugEnabled
                                    worldHeight: worldItem.height
                                    width: model.size.width * app.gridSize
                                    height: model.size.height * app.gridSize
                                    x: model.position.x * app.gridSize
                                    y: model.position.y * app.gridSize
                                    z: worldItem.calculateLayerValue(model.layer, y, height, worldItem.height)
                                }
                            }

                            Repeater {
                                id: itemsRepeater
                                model: Game.mapEditor.activeItems
                                delegate: GameItem {
                                    gameItem: Game.mapEditor.activeItems.get(model.index)
                                    itemDebugEnabled: Game.settings.itemDebugEnabled
                                    width: model.size.width * app.gridSize
                                    height: model.size.height * app.gridSize
                                    x: model.position.x * app.gridSize
                                    y: model.position.y * app.gridSize
                                    z: worldItem.calculateLayerValue(model.layer, y, height, worldItem.height)
                                }
                            }

                            Repeater {
                                id: characersRepeater
                                model: Game.mapEditor.activeCharacters
                                delegate: CharacterItem {
                                    character: Game.mapEditor.activeCharacters.get(model.index)
                                    itemDebugEnabled: Game.settings.itemDebugEnabled
                                    width: model.size.width * app.gridSize
                                    height: model.size.height * app.gridSize
                                    x: model.position.x * app.gridSize
                                    y: model.position.y * app.gridSize
                                    z: worldItem.calculateLayerValue(model.layer, y, height, worldItem.height)
                                }
                            }

                            Repeater {
                                id: enemiesRepeater
                                model: Game.mapEditor.activeEnemies
                                delegate: EnemyItem {
                                    itemDebugEnabled: Game.settings.itemDebugEnabled
                                    enemy: Game.mapEditor.activeEnemies.get(model.index)
                                    width: model.size.width * app.gridSize
                                    height: model.size.height * app.gridSize
                                    x: model.position.x * app.gridSize
                                    y: model.position.y * app.gridSize
                                    z: worldItem.calculateLayerValue(model.layer, y, height, worldItem.height)
                                }
                            }

                            GameItem {
                                id: temporaryItem
                                gameItem: Game.mapEditor.selectedGameItem
                                itemDebugEnabled: Game.settings.itemDebugEnabled
                                visible: editorMouseArea.containsMouse &&
                                         Game.mapEditor.tool == GameMapEditor.ToolPlace

                                width: gameItem ? gameItem.size.width * app.gridSize : 0
                                height: gameItem ? gameItem.size.height * app.gridSize : 0
                                z: temporaryItem.y + temporaryItem.height
                            }
                        }
                    }

                    DebugDraw {
                        id: debugDraw
                        world: physicsWorld
                        opacity: 0.4
                        visible: Game.settings.physicsDebugEnabled
                    }

                    MouseArea {
                        id: editorMouseArea
                        anchors.fill: parent
                        enabled: Game.mapEditor.tool !== GameMapEditor.ToolMove
                        hoverEnabled: true
                        //preventStealing: true

                        onMouseXChanged: {
                            //console.log("--> mouse position changed", mouseX, mouseY)

                            switch (Game.mapEditor.tool) {
                            case GameMapEditor.ToolPlace:
                                mainItem.updatePositions()
                                break;
                            case GameMapEditor.ToolSelect:
                                mainItem.selectItemUnderMouse()
                                break;
                            case GameMapEditor.ToolMove:

                                break;
                            }

                        }

                        onMouseYChanged: {
                            //console.log("--> mouse position changed", mouseX, mouseY)

                            switch (Game.mapEditor.tool) {
                            case GameMapEditor.ToolPlace:
                                mainItem.updatePositions()
                                break;
                            case GameMapEditor.ToolSelect:
                                mainItem.selectItemUnderMouse()
                                break;
                            case GameMapEditor.ToolMove:

                                break;
                            }

                        }

                        onClicked: {
                            switch (Game.mapEditor.tool) {
                            case GameMapEditor.ToolPlace:
                                if (temporaryItem) {
                                    var position = Qt.point(temporaryItem.x / app.gridSize, temporaryItem.y / app.gridSize)
                                    Game.mapEditor.placeItemOnMap(temporaryItem.gameItem.resourcePath, position)
                                }
                                break;
                            case GameMapEditor.ToolSelect:

                                break;
                            case GameMapEditor.ToolMove:

                                break;
                            }
                        }
                    }
                }
            }

            Item {
                id: toolBar
                Layout.fillWidth: true
                Layout.preferredHeight: 40

                Row {
                    id: toolsRow
                    spacing: app.margins
                    anchors.fill: parent
                    anchors.leftMargin: app.margins / 2
                    anchors.rightMargin: app.margins / 2

                    MapEditorToolButton {
                        id: selectToolButton
                        width: 40
                        height: width
                        title: qsTr("Select tool")
                        selected: Game.mapEditor.tool === GameMapEditor.ToolSelect
                        iconSource: dataDirectory + "/icons/select.svg"
                        onClicked: Game.mapEditor.tool = GameMapEditor.ToolSelect
                    }

                    MapEditorToolButton {
                        id: placeToolButton
                        width: 40
                        height: width
                        title: qsTr("Place tool")
                        selected: Game.mapEditor.tool === GameMapEditor.ToolPlace
                        iconSource: dataDirectory + "/icons/save-to.svg"
                        onClicked: Game.mapEditor.tool = GameMapEditor.ToolPlace
                    }

                    MapEditorToolButton {
                        id: moveToolButton
                        width: 40
                        height: width
                        title: qsTr("Move tool")
                        selected: Game.mapEditor.tool === GameMapEditor.ToolMove
                        iconSource: dataDirectory + "/icons/view-fullscreen.svg"
                        onClicked: Game.mapEditor.tool = GameMapEditor.ToolMove
                    }

                    Item {
                        width: 40
                        height: width
                        Rectangle {
                            anchors.centerIn: parent
                            color: "white"
                            height: parent.height
                            width: 2
                            radius: 1
                        }
                    }


                    MapEditorToolButton {
                        id: itemDebugToolButton
                        width: 40
                        height: width
                        toggleButton: true
                        baseColor: "green"
                        title: qsTr("Item debug")
                        iconSource: dataDirectory + "/icons/browser-tabs.svg"
                        onEnabledChanged: {
                            console.log("Item debug enabled changed", enabled)
                            Game.settings.itemDebugEnabled = enabled
                        }

                        Component.onCompleted: itemDebugToolButton.enabled = Game.settings.itemDebugEnabled
                    }

                    MapEditorToolButton {
                        id: physicsDebugToolButton
                        width: 40
                        height: width
                        toggleButton: true
                        baseColor: "green"
                        title: qsTr("Physics debug")
                        iconSource: dataDirectory + "/icons/torch-on.svg"
                        onEnabledChanged: {
                            console.log("Physics debug enabled changed", enabled)
                            Game.settings.physicsDebugEnabled = enabled
                        }

                        Component.onCompleted: physicsDebugToolButton.enabled = Game.settings.physicsDebugEnabled
                    }

                    MapEditorToolButton {
                        id: gridSnappingToolButton
                        width: 40
                        height: width
                        toggleButton: true
                        baseColor: "green"
                        title: qsTr("Grid snapping")
                        iconSource: dataDirectory + "/icons/camera-grid.svg"
                        onEnabledChanged: {
                            console.log("Grid snaping enabled changed", enabled)
                            Game.settings.gridSnapping = enabled
                        }

                        Component.onCompleted: gridSnappingToolButton.enabled = Game.settings.gridSnapping
                    }

                    Item {
                        width: 40
                        height: width
                        Rectangle {
                            anchors.centerIn: parent
                            color: "white"
                            height: parent.height
                            width: 2
                            radius: 1
                        }
                    }

                    MapEditorToolButton {
                        id: deleteAllButton
                        width: 40
                        height: width
                        baseColor: "red"
                        title: qsTr("Delete all")
                        iconSource: dataDirectory + "/icons/delete.svg"
                        onClicked: {
                            console.log("Delete all clicked")
                            Game.mapEditor.deleteAll()
                        }
                    }

                    Item {
                        width: 40
                        height: width
                        Rectangle {
                            anchors.centerIn: parent
                            color: "white"
                            height: parent.height
                            width: 2
                            radius: 1
                        }
                    }

                    MapEditorToolButton {
                        id: saveMapButton
                        width: 40
                        height: width
                        title: qsTr("Save map")
                        baseColor: "green"
                        iconSource: dataDirectory + "/icons/drive-harddisk-symbolic.svg"
                        onClicked: {
                            console.log("Save map clicked")
                            Game.mapEditor.saveMap()
                        }
                    }



                }
            }
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
}

