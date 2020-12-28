import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12
import Chronicles 1.0

Item {
    id: root

    property bool itemDebugEnabled: false
    property bool physicsDebugEnabled: false

    property bool stonedEnabled: false

    property real grayscaleFactor: 0.0
    property real brightness: 1.0
    property real contrast: 1.0
    property real gamma: 1.0

    // Ambient
    property real ambientBrightness: 1.0
    property color ambientLightColor: Qt.rgba(0, 0, 0, 0)
    property color playerLightColor: Qt.rgba(0, 0, 0, 0)

    property bool flamesEnabled: false

    // Weather controls
    property bool rainingEnabled: false
    property bool snowingEnabled: false
    property bool turbulenceEnabled: false

    onAmbientLightColorChanged: console.log("Ambient light color changed", ambientLightColor)
    onPlayerLightColorChanged: console.log("Player light color changed", playerLightColor)

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

            RowLayout {
                height: 40
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: app.margins
                anchors.rightMargin: app.margins

                Label {
                    text: qsTr("Grayscale factor")
                }

                Slider {
                    Layout.fillWidth: true
                    from: 0
                    to: 1
                    value: 1.0
                    onValueChanged: root.grayscaleFactor = value
                    Component.onCompleted: value = root.grayscaleFactor
                }
            }

            RowLayout {
                height: 40
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: app.margins
                anchors.rightMargin: app.margins

                Label {
                    text: qsTr("Brightness")
                }

                Slider {
                    Layout.fillWidth: true
                    from: 0.0
                    to: 2.0
                    value: 1.0
                    onValueChanged: root.brightness = value
                    Component.onCompleted: value = root.brightness
                }
            }


            RowLayout {
                height: 40
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: app.margins
                anchors.rightMargin: app.margins

                Label {
                    text: qsTr("Contrast")
                }

                Slider {
                    Layout.fillWidth: true
                    from: 0
                    to: 2.0
                    value: 1.0
                    onValueChanged: root.contrast = value
                    Component.onCompleted: value = root.contrast
                }
            }

            RowLayout {
                height: 40
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: app.margins
                anchors.rightMargin: app.margins

                Label {
                    text: qsTr("Gamma")
                }

                Slider {
                    Layout.fillWidth: true
                    from: 0
                    to: 2.0
                    value: 1.0
                    onValueChanged: root.gamma = value
                    Component.onCompleted: value = root.gamma
                }
            }

            RowLayout {
                height: 40
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: app.margins
                anchors.rightMargin: app.margins

                Label {
                    text: qsTr("Ambient brightness")
                }

                Slider {
                    Layout.fillWidth: true
                    from: 0
                    to: 1
                    value: 1.0
                    onValueChanged: root.ambientBrightness = value
                    Component.onCompleted: value = root.ambientBrightness
                }
            }

            RowLayout {
                height: 40
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: app.margins
                anchors.rightMargin: app.margins

                Label {
                    text: qsTr("Ambient color") + " | " + root.ambientLightColor
                    Layout.fillWidth: true
                }

                Button {
                    Layout.preferredHeight: 40
                    Layout.fillWidth: true
                    text: qsTr("Pick ambient color")
                    onClicked: {
                        Game.debugging = false
                        ambientLightPopup.open()
                    }
                }
            }


            RowLayout {
                height: 40
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: app.margins
                anchors.rightMargin: app.margins

                Label {
                    text: qsTr("Player light color") + " | " + root.playerLightColor
                    Layout.fillWidth: true
                }

                Button {
                    Layout.preferredHeight: 40
                    Layout.fillWidth: true
                    text: qsTr("Pick player light color")
                    onClicked: {
                        Game.debugging = false
                        playerLightPopup.open()
                    }
                }
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
