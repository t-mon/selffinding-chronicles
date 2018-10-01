import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0

import Chronicles 1.0

import "../components"

Item {
    id: root

    property QtObject field: null

    Image {
        id: backgroundImage
        anchors.fill: parent
        source: dataDirectory + field.imageName
    }

    Rectangle {
        id: currentFieldIndicator
        anchors.fill: parent
        color: "steelblue"
        opacity: 0.5
        visible: Game.debugging && field.inPlayerRange && !field.playerOnField
    }

    Rectangle {
        id: inPlayerRangeIndicator
        anchors.fill: parent
        color: "white"
        opacity: 0.5
        visible: Game.debugging && field.playerOnField
    }

    Rectangle {
        id: unaccessableIndicator
        anchors.fill: parent
        color: "red"
        opacity: Game.debugging && !field.accessible ? 0.2 : 0
    }

    Loader {
        id: itemLoader
        anchors.fill: parent
    }

    Binding{
        target: itemLoader.item
        property: "field"
        value: root.field
        when: itemLoader.status == Loader.Ready
    }

    Component.onCompleted: {
//        if (field.gameItem) {
//            itemLoader.source = Qt.resolvedUrl("gameitems/GameItem.qml", { field: root.field, cellSize: root.cellSize } )
//        }
    }
}

