import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0

import Chronicles 1.0

Item {
    id: root

    property bool itemDebugEnabled: false
    property bool physicsDebugEnabled: false
    property bool stonedEnabled: false
    property bool grayscaleEnabled: false
    property bool magicEnabled: false
    property bool flamesEnabled: false
    property bool rainingEnabled: false
    property bool snowingEnabled: false
    property bool turbulenceEnabled: false

    Flickable {
        width: root.width / 2
        height: parent.height
        anchors.right: parent.right
        anchors.top: parent.top
        contentHeight: optionsColumn.height

        Rectangle {
            anchors.fill: parent
            color: "white"
            opacity: 0.6
        }

        Column {
            id: optionsColumn
            width: parent.width

            SwitchDelegate {
                width: parent.width
                height: 40
                text: qsTr("Item debug")
                onCheckedChanged: root.itemDebugEnabled = !root.itemDebugEnabled
                Component.onCompleted: checked = root.itemDebugEnabled
            }

            SwitchDelegate {
                width: parent.width
                height: 40
                text: qsTr("Physics debug")
                onCheckedChanged: root.physicsDebugEnabled = !root.physicsDebugEnabled
                Component.onCompleted: checked = root.physicsDebugEnabled
            }

            SwitchDelegate {
                width: parent.width
                height: 40
                text: qsTr("Stonend")
                onCheckedChanged: root.stonedEnabled = !root.stonedEnabled
                Component.onCompleted: checked = root.stonedEnabled
            }

            SwitchDelegate {
                width: parent.width
                height: 40
                text: qsTr("Grayscale")
                onCheckedChanged: root.grayscaleEnabled = !root.grayscaleEnabled
                Component.onCompleted: checked = root.grayscaleEnabled
            }

            SwitchDelegate {
                width: parent.width
                height: 40
                text: qsTr("Magic")
                onCheckedChanged: root.magicEnabled = !root.magicEnabled
                Component.onCompleted: checked = root.magicEnabled
            }

            SwitchDelegate {
                width: parent.width
                height: 40
                text: qsTr("Burning arrows")
                onCheckedChanged: root.flamesEnabled = !root.flamesEnabled
                Component.onCompleted: checked = root.flamesEnabled
            }

            RowLayout {
                height: 40
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: app.margins
                anchors.rightMargin: app.margins

                Label {
                    text: qsTr("Gamespeed")
                }

                Slider {
                    Layout.fillWidth: true
                    from: 0.1
                    to: 1
                    stepSize: 0.1
                    value: 1.0
                    onValueChanged: app.gameSpeedFactor = value
                    Component.onCompleted: value = app.gameSpeedFactor
                }
            }

            SwitchDelegate {
                width: parent.width
                height: 40
                text: qsTr("Raining")
                onCheckedChanged: root.rainingEnabled = !root.rainingEnabled
                Component.onCompleted: checked = root.rainingEnabled
            }

            SwitchDelegate {
                width: parent.width
                height: 40
                text: qsTr("Snowing")
                onCheckedChanged: root.snowingEnabled = !root.snowingEnabled
                Component.onCompleted: checked = root.snowingEnabled
            }
        }
    }
}
