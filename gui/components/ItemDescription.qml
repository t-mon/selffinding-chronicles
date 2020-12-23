import QtQuick 2.5
import QtQuick.Controls 2.2
import QtGraphicalEffects 1.0

import Chronicles 1.0


Item {
    id: root

    property alias text: description.text

    implicitHeight: description.implicitHeight
    implicitWidth: description.implicitWidth

    Behavior on opacity {
        NumberAnimation {
            duration: 300
        }
    }

    GameLabel {
        id: description
        color: "black"
        anchors.horizontalCenter: root.horizontalCenter
        anchors.top: root.top
    }

    Glow {
        anchors.fill: description
        color: "#cdffffff"
        source: description
        radius: 3
        samples: 8
    }
}

