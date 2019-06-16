import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0

import Chronicles 1.0

Item {
    id: root

    property bool itemDebugEnabled: false
    property bool physicsDebugEnabled: true
    property bool stonedEnabled: false
    property bool grayscaleEnabled: false
    property bool flamesEnabled: true
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
                    id: itemDebugButton
                    width: parent.width
                    text: root.itemDebugEnabled ? "Item debug OFF" : "Item debug ON"
                    color: root.itemDebugEnabled ? "green" : "white"
                    onClicked: root.itemDebugEnabled = !root.itemDebugEnabled
                }

                GameButton {
                    id: physicsDebugEnabledButton
                    width: parent.width
                    text: root.physicsDebugEnabled ? "Physics debug OFF" : "Physics debug ON"
                    color: root.physicsDebugEnabled ? "green" : "white"
                    onClicked: root.physicsDebugEnabled = !root.physicsDebugEnabled
                }

                GameButton {
                    id: stonedEnabledButton
                    width: parent.width
                    text: root.stonedEnabled ? "Stoned OFF" : "Stoned ON"
                    color: root.stonedEnabled ? "green" : "white"
                    onClicked: root.stonedEnabled = !root.stonedEnabled
                }

                GameButton {
                    id: grayscaleEnabledButton
                    width: parent.width
                    text: root.grayscaleEnabled ? "Grayscale OFF" : "Grayscale ON"
                    color: root.grayscaleEnabled ? "green" : "white"
                    onClicked: root.grayscaleEnabled = !root.grayscaleEnabled
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
