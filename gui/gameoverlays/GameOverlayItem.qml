import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0

import Chronicles 1.0

import "../components"

Item {
    id: root

    Rectangle {
        id: backgroundRectangle
        anchors.fill: parent
        color: "black"
        opacity: 0.2
    }

    onOpacityChanged: {
        if (opacity === 0) {
            root.visible = false
        } else {
            root.visible = true
        }
    }

    Behavior on opacity {
        NumberAnimation { duration: 250 }
    }

}
