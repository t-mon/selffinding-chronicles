import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0

import Chronicles 1.0

Item {
    id: root

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
