import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.0

import "../../components"

Item {
    id: root

    property color currentColor: Qt.rgba(red, green, blue, alpha)

    function setInitialColor(color) {

    }

    property real red: 0
    property real green: 0
    property real blue: 0
    property real alpha: 255

    ColumnLayout {
        anchors.fill: parent

        ComboBox {
            id: modeComboBox
            Layout.fillWidth: true
            model: [ qsTr("RGBA"), qsTr("HSV") ]
        }

        Item {
            id: colorRectangle
            Layout.preferredHeight: 40
            Layout.fillWidth: true

            Rectangle {
                anchors.fill: parent
                border.color: "black"
                border.width: 2
                color: Qt.rgba(root.red, root.green, root.blue, root.alpha)
            }
        }

        GridLayout {
            Layout.fillWidth: true
            columns: 2

            GameLabel { text: "R"; color: "white"; Layout.preferredWidth: 5 }
            Slider {
                id: redSlider
                Layout.fillWidth: true
                from: 0
                to: 256
                stepSize: 1
                onValueChanged: root.red = value * 100 / 25500
                background: LinearGradient {
                    anchors.fill: parent
                    start: Qt.point(0, 0)
                    end: Qt.point(width, 0)
                    gradient: Gradient {
                        GradientStop { position: 0.0; color: Qt.rgba(0, root.green, root.blue, 1) }
                        GradientStop { position: 1.0; color: Qt.rgba(1, root.green, root.blue, 1) }
                    }
                }

                Component.onCompleted: value = root.red
            }


            GameLabel { text: "G"; color: "white"; Layout.preferredWidth: 5 }
            Slider {
                id: greenSlider
                Layout.fillWidth: true
                from: 0
                to: 256
                stepSize: 1
                onValueChanged: root.green = value * 100 / 25500
                background: LinearGradient {
                    anchors.fill: parent
                    start: Qt.point(0, 0)
                    end: Qt.point(width, 0)
                    gradient: Gradient {
                        GradientStop { position: 0.0; color: Qt.rgba(root.red, 0, root.blue, 1) }
                        GradientStop { position: 1.0; color: Qt.rgba(root.red, 1, root.blue, 1) }
                    }
                }
                Component.onCompleted: value = root.green
            }

            GameLabel { text: "B"; color: "white"; Layout.preferredWidth: 5 }
            Slider {
                id: blueSlider
                Layout.fillWidth: true
                from: 0
                to: 255
                stepSize: 1
                onValueChanged: root.blue = value * 100 / 25500
                background: LinearGradient {
                    anchors.fill: parent
                    start: Qt.point(0, 0)
                    end: Qt.point(width, 0)

                    gradient: Gradient {
                        GradientStop { position: 0.0; color: Qt.rgba(root.red, root.green, 0, 1) }
                        GradientStop { position: 1.0; color: Qt.rgba(root.red, root.green, 1, 1) }
                    }
                }
                Component.onCompleted: value = root.blue
            }

            GameLabel { text: "A"; color: "white"; Layout.preferredWidth: 5 }
            Slider {
                id: aplhaSlider
                Layout.fillWidth: true
                from: 0
                to: 255
                stepSize: 1
                onValueChanged: root.alpha = value * 100 / 25500
                background: LinearGradient {
                    anchors.fill: parent
                    start: Qt.point(0, 0)
                    end: Qt.point(width, 0)

                    gradient: Gradient {
                        GradientStop { position: 0.0; color: Qt.rgba(root.red, root.green, root.blue, 0) }
                        GradientStop { position: 1.0; color: Qt.rgba(root.red, root.green, root.blue, 1) }
                    }
                }
                Component.onCompleted: value = root.alpha
            }

            Item {
                Layout.fillHeight: true
                Layout.fillWidth: true
            }
        }
    }
}
