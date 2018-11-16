import QtQuick 2.5
import QtQuick.Controls 2.2

ToolButton {
    property alias imageSource: image.source
//    property alias color: image.color
//    property alias keyColor: image.keyColor

    contentItem: Item {
        height: 20
        width: 20
        Image {
            id: image
            anchors.fill: parent
            anchors.margins: app.margins / 2
        }
    }
}
