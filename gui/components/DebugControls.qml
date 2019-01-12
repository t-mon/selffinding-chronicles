import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0

import Chronicles 1.0

Item {
    id: root

    property bool drugged: false
    property bool flamesEnabled: false

    ColumnLayout {
        anchors.fill: parent

        GameButton {
            id: druggedButton
            Layout.fillWidth: true
            text: root.drugged ? "Drugged OFF" : "Drugged ON"
            onClicked: root.drugged = !root.drugged
        }

        GameButton {
            id: flamesEnabledButton
            Layout.fillWidth: true
            text: root.flamesEnabled ? "Flames OFF" : "Flames ON"
            onClicked: root.flamesEnabled = !root.flamesEnabled
        }
    }

}
