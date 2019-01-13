import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0

import Chronicles 1.0

Item {
    id: root

    property bool physicsDebug: false
    property bool drugged: false
    property bool flamesEnabled: false
    property bool rainingEnabled: false
    property bool snowingEnabled: false
    property bool turbulenceEnabled: false

    property bool dropDown: false

    height: mainColumn.implicitHeight

    Column {
        id: mainColumn
        width: parent.width
        spacing: app.margins

        GameButton {
            width: parent.width
            text: "Debug options"
            color: root.dropDown ? "steelblue" : "white"
            onClicked: root.dropDown = !root.dropDown
        }

        Item {
            id: menuItem
            width: parent.width
            height: root.dropDown ? optionsColumn.implicitHeight : 0
            visible: root.dropDown


            Column {
                id: optionsColumn
                width: parent.width
                spacing: app.margins

                Rectangle {
                    width: parent.width
                    height: 1
                    radius: height / 2
                }

                GameButton {
                    id: physicsDebugButton
                    width: parent.width
                    text: root.physicsDebug ? "Physics debug OFF" : "Physics debug ON"
                    color: root.physicsDebug ? "green" : "white"
                    onClicked: root.physicsDebug = !root.physicsDebug
                }

                GameButton {
                    id: druggedButton
                    width: parent.width
                    text: root.drugged ? "Drugged OFF" : "Drugged ON"
                    color: root.drugged ? "green" : "white"
                    onClicked: root.drugged = !root.drugged
                }

                GameButton {
                    id: flamesEnabledButton
                    width: parent.width
                    text: root.flamesEnabled ? "Flame arrow OFF" : "Flames arrow ON"
                    color: root.flamesEnabled ? "green" : "white"
                    onClicked: root.flamesEnabled = !root.flamesEnabled
                }

                Rectangle {
                    width: parent.width
                    height: 1
                    radius: height / 2
                }

                GameButton {
                    id: rainingEnabledButton
                    width: parent.width
                    text: root.rainingEnabled ? "Raining OFF" : "Raining ON"
                    color: root.rainingEnabled ? "green" : "white"
                    onClicked: root.rainingEnabled = !root.rainingEnabled
                }

                GameButton {
                    id: snowingEnabledButton
                    width: parent.width
                    text: root.snowingEnabled ? "Snowing OFF" : "Snowing ON"
                    color: root.snowingEnabled ? "green" : "white"
                    onClicked: root.snowingEnabled = !root.snowingEnabled
                }

                GameButton {
                    id: turbulenceEnabledButton
                    width: parent.width
                    text: root.turbulenceEnabled ? "Turbulence OFF" : "Turbulence ON"
                    color: root.turbulenceEnabled ? "green" : "white"
                    onClicked: root.turbulenceEnabled = !root.turbulenceEnabled
                }
            }
        }

    }

}
