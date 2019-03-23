import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0

import Chronicles 1.0

Item {
    id: root

    property string imageSource
    property bool selected: false
    property real rotation: 0

    Rectangle {
        id: backgroundRectangle
        anchors.fill: parent
        anchors.margins: app.margins / 2
        color: selected ? "steelblue" : "gray"
        border.color: "white"
        border.width: app.borderWidth
        radius: app.margins / 2
        opacity: 0.4
    }

    Image {
        id: itemImage
        anchors.fill: backgroundRectangle
        anchors.margins: parent.width / 8
        rotation: root.rotation
        source: root.imageSource? root.imageSource : ""
    }

}
