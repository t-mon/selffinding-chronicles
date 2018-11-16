import QtQuick 2.5
import QtQuick.Controls 2.2

import Chronicles 1.0

Button {
    id: root

    background: Rectangle {
        implicitWidth: app.gridSize * 4
        implicitHeight: app.gridSize
        border.width: root.activeFocus ? 2 : 1
        border.color: "#888"
        radius: 4
    }

}
