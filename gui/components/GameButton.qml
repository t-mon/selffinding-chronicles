import QtQuick 2.5
import QtQuick.Controls 2.2

import Chronicles 1.0

Button {
    id: root

    font.pixelSize: app.largeFont
    font.family: app.fontFamily

    background: Rectangle {
        implicitWidth: app.gridSize * 4
        implicitHeight: app.gridSize
        border.width: root.activeFocus ? 2 : 1
        border.color: "#888888"
        radius: 4
    }

}
