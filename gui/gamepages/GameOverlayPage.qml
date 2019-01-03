import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0

import Chronicles 1.0

import "../components"

Item {
    id: root

    onOpacityChanged: {
        if (opacity === 0) {
            root.visible = false
        } else {
            root.visible = true
        }
    }

    Behavior on opacity {
        NumberAnimation { duration: 150 }
    }

}
