import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0

import Chronicles 1.0

import "../components"

Item {
    id: root

    property QtObject field: null
    antialiasing: app.antialiasing

    Image {
        id: backgroundImage
        anchors.fill: parent
        source: dataDirectory + field.imageName
    }

    Rectangle {
        id: inPlayerRangeIndicator
        anchors.fill: parent
        color: "white"
        opacity: 0.5
        visible: Game.debugging && field.playerOnField
    }
}

