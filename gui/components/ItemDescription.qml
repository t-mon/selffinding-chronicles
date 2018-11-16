import QtQuick 2.5
import QtQuick.Controls 2.2

import Chronicles 1.0

GameLabel {
    id: root
    color: "black"
    Behavior on opacity {
        NumberAnimation {
            duration: 300
        }
    }
}

