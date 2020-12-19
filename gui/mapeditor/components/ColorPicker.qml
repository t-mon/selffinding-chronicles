import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.0

import "../../components"

Item {
    id: root

    property color color

    property real pickerHeight: 35

    ColumnLayout {
        anchors.fill: parent

        ComboBox {
            id: modeComboBox
            Layout.fillWidth: true
            model: [ qsTr("RGB"), qsTr("HSV"), qsTr("HSL") ]
        }

        Item {
            id: colorRectangle
            Layout.preferredHeight: root.pickerHeight
            Layout.fillWidth: true

            Rectangle {
                anchors.fill: parent
                border.color: "black"
                border.width: 2
                color: root.color

                Label {
                    anchors.centerIn: parent
                    text: root.color
                    style: Text.Outline
                    color: "white"
                    styleColor: "black"
                }
            }
        }

        Loader {
            Layout.fillWidth: true
            Layout.fillHeight: true
            sourceComponent: modeComboBox.currentIndex === 0 ? rgbComponent : (modeComboBox.currentIndex === 1 ? hsvComponent : hslComponent)

            Component {
                id: rgbComponent
                Item {
                    GridLayout {
                        anchors.fill: parent
                        columns: 2

                        GameLabel { text: "R"; color: "white"; Layout.preferredWidth: 15 }
                        Item {
                            Layout.fillWidth: true
                            Layout.preferredHeight: root.pickerHeight + app.margins / 2

                            Slider {
                                id: redSlider
                                anchors.left: parent.left
                                anchors.right: parent.right
                                anchors.verticalCenter: parent.verticalCenter
                                from: 0
                                to: 255
                                stepSize: 1
                                value: color.r * 255
                                onValueChanged: {
                                    if (color.r !== value / 255) {
                                        color.r = value / 255
                                    }
                                }
                                background: LinearGradient {
                                    id: redGradient
                                    property color startColor: Qt.rgba(0, color.g, color.b, 1)
                                    property color stopColor: Qt.rgba(1, color.g, color.b, 1)
                                    x: redSlider.leftPadding
                                    width: redSlider.width - redSlider.leftPadding - redSlider.rightPadding
                                    height: root.pickerHeight
                                    anchors.verticalCenter: parent.verticalCenter
                                    start: Qt.point(0, 0)
                                    end: Qt.point(width, 0)
                                    gradient: Gradient {
                                        GradientStop { position: 0.0; color: redGradient.startColor }
                                        GradientStop { position: 1.0; color: redGradient.stopColor }
                                    }
                                }

                                handle: Rectangle {
                                    x: redSlider.leftPadding + redSlider.visualPosition * (redSlider.availableWidth - width)
                                    y: redSlider.availableHeight / 2 - height / 2
                                    anchors.verticalCenter: parent.verticalCenter
                                    implicitWidth: 5
                                    implicitHeight: root.pickerHeight
                                    color: redSlider.pressed ? "#f0f0f0" : "#f6f6f6"
                                    border.color: "#bdbebf"
                                }
                            }

                        }

                        GameLabel { text: "G"; color: "white"; Layout.preferredWidth: 15 }
                        Item {
                            Layout.fillWidth: true
                            Layout.preferredHeight: root.pickerHeight + app.margins / 2

                            Slider {
                                id: greenSlider
                                anchors.left: parent.left
                                anchors.right: parent.right
                                anchors.verticalCenter: parent.verticalCenter
                                from: 0
                                to: 255
                                stepSize: 1
                                value: color.g * 255
                                onValueChanged: {
                                    if (color.g !== value / 255) {
                                        color.g = value / 255
                                    }
                                }
                                background: LinearGradient {
                                    id: greenGradient
                                    property color startColor: Qt.rgba(color.r, 0, color.b, 1)
                                    property color stopColor: Qt.rgba(color.r, 1, color.b, 1)
                                    x: greenSlider.leftPadding
                                    width: greenSlider.width - greenSlider.leftPadding - greenSlider.rightPadding
                                    height: root.pickerHeight
                                    anchors.verticalCenter: parent.verticalCenter
                                    start: Qt.point(0, 0)
                                    end: Qt.point(width, 0)
                                    gradient: Gradient {
                                        GradientStop { position: 0.0; color: greenGradient.startColor }
                                        GradientStop { position: 1.0; color: greenGradient.stopColor }
                                    }
                                }

                                handle: Rectangle {
                                    x: greenSlider.leftPadding + greenSlider.visualPosition * (greenSlider.availableWidth - width)
                                    y: greenSlider.availableHeight / 2 - height / 2
                                    anchors.verticalCenter: parent.verticalCenter
                                    implicitWidth: 5
                                    implicitHeight: root.pickerHeight
                                    color: greenSlider.pressed ? "#f0f0f0" : "#f6f6f6"
                                    border.color: "#bdbebf"
                                }
                            }
                        }

                        GameLabel { text: "B"; color: "white"; Layout.preferredWidth: 15 }
                        Item {
                            Layout.fillWidth: true
                            Layout.preferredHeight: root.pickerHeight + app.margins / 2

                            Slider {
                                id: blueSlider
                                anchors.left: parent.left
                                anchors.right: parent.right
                                anchors.verticalCenter: parent.verticalCenter
                                from: 0
                                to: 255
                                stepSize: 1
                                value: color.b * 255
                                onValueChanged: {
                                    if (color.b !== value / 255) {
                                        color.b = value / 255
                                    }
                                }

                                background: LinearGradient {
                                    id: blueGradient
                                    property color startColor: Qt.rgba(color.r, color.g, 0, 1)
                                    property color stopColor: Qt.rgba(color.r, color.g, 1, 1)
                                    x: blueSlider.leftPadding
                                    width: blueSlider.width - blueSlider.leftPadding - blueSlider.rightPadding
                                    height: root.pickerHeight
                                    anchors.verticalCenter: parent.verticalCenter
                                    start: Qt.point(0, 0)
                                    end: Qt.point(width, 0)
                                    gradient: Gradient {
                                        GradientStop { position: 0.0; color: blueGradient.startColor }
                                        GradientStop { position: 1.0; color: blueGradient.stopColor }
                                    }
                                }

                                handle: Rectangle {
                                    x: blueSlider.leftPadding + blueSlider.visualPosition * (blueSlider.availableWidth - width)
                                    y: blueSlider.availableHeight / 2 - height / 2
                                    anchors.verticalCenter: parent.verticalCenter
                                    implicitWidth: 5
                                    implicitHeight: root.pickerHeight
                                    color: blueSlider.pressed ? "#f0f0f0" : "#f6f6f6"
                                    border.color: "#bdbebf"
                                }
                            }

                        }



                        GameLabel { text: "A"; color: "white"; Layout.preferredWidth: 15 }
                        Item {
                            Layout.fillWidth: true
                            Layout.preferredHeight: root.pickerHeight + app.margins / 2

                            Slider {
                                id: alphaSlider
                                anchors.left: parent.left
                                anchors.right: parent.right
                                anchors.verticalCenter: parent.verticalCenter
                                from: 0
                                to: 255
                                stepSize: 1
                                value: color.a * 255
                                onValueChanged: {
                                    if (color.a !== value / 255) {
                                        color.a = value / 255
                                    }
                                }
                                background: LinearGradient {
                                    id: alphaGradient
                                    property color startColor: Qt.rgba(color.r, color.g, color.b, 0)
                                    property color stopColor: Qt.rgba(color.r, color.g, color.b, 1)
                                    x: alphaSlider.leftPadding
                                    width: alphaSlider.width - alphaSlider.leftPadding - alphaSlider.rightPadding
                                    height: root.pickerHeight
                                    anchors.verticalCenter: parent.verticalCenter
                                    start: Qt.point(0, 0)
                                    end: Qt.point(width, 0)
                                    gradient: Gradient {
                                        GradientStop { position: 0.0; color: alphaGradient.startColor }
                                        GradientStop { position: 1.0; color: alphaGradient.stopColor }
                                    }
                                }

                                handle: Rectangle {
                                    x: alphaSlider.leftPadding + alphaSlider.visualPosition * (alphaSlider.availableWidth - width)
                                    y: alphaSlider.availableHeight / 2 - height / 2
                                    anchors.verticalCenter: parent.verticalCenter
                                    implicitWidth: 5
                                    implicitHeight: root.pickerHeight
                                    color: alphaSlider.pressed ? "#f0f0f0" : "#f6f6f6"
                                    border.color: "#bdbebf"
                                }
                            }
                        }
                    }
                }
            }

            Component {
                id: hsvComponent

                Item {
                    GridLayout {
                        anchors.fill: parent
                        columns: 2

                        GameLabel { text: "H"; color: "white"; Layout.preferredWidth: 15 }
                        Item {
                            Layout.fillWidth: true
                            Layout.preferredHeight: root.pickerHeight + app.margins / 2

                            Slider {
                                id: hueSlider
                                anchors.left: parent.left
                                anchors.right: parent.right
                                anchors.verticalCenter: parent.verticalCenter
                                from: 0
                                to: 255
                                stepSize: 1
                                value: color.hsvHue * 255
                                onValueChanged: {
                                    if (color.hsvHue !== value / 255) {
                                        color.hsvHue = value / 255
                                    }
                                }

                                background: LinearGradient {
                                    id: hueGradient
                                    x: hueSlider.leftPadding
                                    width: hueSlider.width - hueSlider.leftPadding - hueSlider.rightPadding
                                    height: root.pickerHeight
                                    anchors.verticalCenter: parent.verticalCenter
                                    start: Qt.point(0, 0)
                                    end: Qt.point(width, 0)
                                    gradient: Gradient {
                                        GradientStop { position: 0.0;  color: "#FF0000" }
                                        GradientStop { position: 0.16; color: "#FFFF00" }
                                        GradientStop { position: 0.33; color: "#00FF00" }
                                        GradientStop { position: 0.5;  color: "#00FFFF" }
                                        GradientStop { position: 0.76; color: "#0000FF" }
                                        GradientStop { position: 0.85; color: "#FF00FF" }
                                        GradientStop { position: 1.0;  color: "#FF0000" }
                                    }
                                }

                                handle: Rectangle {
                                    x: hueSlider.leftPadding + hueSlider.visualPosition * (hueSlider.availableWidth - width)
                                    y: hueSlider.availableHeight / 2 - height / 2
                                    anchors.verticalCenter: parent.verticalCenter
                                    implicitWidth: 5
                                    implicitHeight: root.pickerHeight
                                    color: hueSlider.pressed ? "#f0f0f0" : "#f6f6f6"
                                    border.color: "#bdbebf"
                                }
                            }
                        }


                        GameLabel { text: "S"; color: "white"; Layout.preferredWidth: 15 }
                        Item {
                            Layout.fillWidth: true
                            Layout.preferredHeight: root.pickerHeight + app.margins / 2

                            Slider {
                                id: saturationSlider
                                anchors.left: parent.left
                                anchors.right: parent.right
                                anchors.verticalCenter: parent.verticalCenter
                                from: 0
                                to: 255
                                stepSize: 1
                                value: color.hsvSaturation * 255
                                onValueChanged: {
                                    if (color.hsvSaturation !== value / 255) {
                                        color.hsvSaturation = value / 255
                                    }
                                }
                                background: LinearGradient {
                                    id: saturationGradient
                                    property color startColor: Qt.hsva(color.hsvHue, 0, color.hsvValue, 1)
                                    property color stopColor: Qt.hsva(color.hsvHue, 1, color.hsvValue, 1)
                                    x: saturationSlider.leftPadding
                                    width: saturationSlider.width - saturationSlider.leftPadding - saturationSlider.rightPadding
                                    height: root.pickerHeight
                                    anchors.verticalCenter: parent.verticalCenter
                                    start: Qt.point(0, 0)
                                    end: Qt.point(width, 0)
                                    gradient: Gradient {
                                        GradientStop { position: 0.0; color: saturationGradient.startColor }
                                        GradientStop { position: 1.0; color: saturationGradient.stopColor }
                                    }
                                }

                                handle: Rectangle {
                                    x: saturationSlider.leftPadding + saturationSlider.visualPosition * (saturationSlider.availableWidth - width)
                                    y: saturationSlider.availableHeight / 2 - height / 2
                                    anchors.verticalCenter: parent.verticalCenter
                                    implicitWidth: 5
                                    implicitHeight: root.pickerHeight
                                    color: saturationSlider.pressed ? "#f0f0f0" : "#f6f6f6"
                                    border.color: "#bdbebf"
                                }
                            }
                        }


                        GameLabel { text: "V"; color: "white"; Layout.preferredWidth: 15 }
                        Item {
                            Layout.fillWidth: true
                            Layout.preferredHeight: root.pickerHeight + app.margins / 2

                            Slider {
                                id: valueSlider
                                anchors.left: parent.left
                                anchors.right: parent.right
                                anchors.verticalCenter: parent.verticalCenter
                                from: 0
                                to: 255
                                stepSize: 1
                                value: color.hsvValue * 255
                                onValueChanged: {
                                    if (color.hsvValue !== value / 255) {
                                        color.hsvValue = value / 255
                                    }
                                }
                                background: LinearGradient {
                                    id: valueGradient
                                    property color startColor: Qt.hsva(color.hsvHue, color.hsvSaturation, 0, 1)
                                    property color stopColor: Qt.hsva(color.hsvHue, color.hsvSaturation, 1, 1)
                                    x: valueSlider.leftPadding
                                    width: valueSlider.width - valueSlider.leftPadding - valueSlider.rightPadding
                                    height: root.pickerHeight
                                    anchors.verticalCenter: parent.verticalCenter
                                    start: Qt.point(0, 0)
                                    end: Qt.point(width, 0)
                                    gradient: Gradient {
                                        GradientStop { position: 0.0; color: valueGradient.startColor }
                                        GradientStop { position: 1.0; color: valueGradient.stopColor }
                                    }
                                }

                                handle: Rectangle {
                                    x: valueSlider.leftPadding + valueSlider.visualPosition * (valueSlider.availableWidth - width)
                                    y: valueSlider.availableHeight / 2 - height / 2
                                    anchors.verticalCenter: parent.verticalCenter
                                    implicitWidth: 5
                                    implicitHeight: root.pickerHeight
                                    color: valueSlider.pressed ? "#f0f0f0" : "#f6f6f6"
                                    border.color: "#bdbebf"
                                }

                            }
                        }


                        GameLabel { text: "A"; color: "white"; Layout.preferredWidth: 15 }
                        Item {
                            Layout.fillWidth: true
                            Layout.preferredHeight: root.pickerHeight + app.margins / 2

                            Slider {
                                id: alphaSlider
                                anchors.left: parent.left
                                anchors.right: parent.right
                                anchors.verticalCenter: parent.verticalCenter
                                from: 0
                                to: 255
                                stepSize: 1
                                value: color.a * 255
                                onValueChanged: {
                                    if (color.a !== value / 255) {
                                        color.a = value / 255
                                    }
                                }
                                background: LinearGradient {
                                    id: alphaGradient
                                    property color startColor: Qt.hsva(color.hsvHue, color.hsvSaturation, color.hsvValue, 0)
                                    property color stopColor: Qt.hsva(color.hsvHue, color.hsvSaturation, color.hsvValue, 1)
                                    x: alphaSlider.leftPadding
                                    width: alphaSlider.width - alphaSlider.leftPadding - alphaSlider.rightPadding
                                    height: root.pickerHeight
                                    anchors.verticalCenter: parent.verticalCenter
                                    start: Qt.point(0, 0)
                                    end: Qt.point(width, 0)
                                    gradient: Gradient {
                                        GradientStop { position: 0.0; color: alphaGradient.startColor }
                                        GradientStop { position: 1.0; color: alphaGradient.stopColor }
                                    }
                                }

                                handle: Rectangle {
                                    x: alphaSlider.leftPadding + alphaSlider.visualPosition * (alphaSlider.availableWidth - width)
                                    y: alphaSlider.availableHeight / 2 - height / 2
                                    anchors.verticalCenter: parent.verticalCenter
                                    implicitWidth: 5
                                    implicitHeight: root.pickerHeight
                                    color: alphaSlider.pressed ? "#f0f0f0" : "#f6f6f6"
                                    border.color: "#bdbebf"
                                }
                            }
                        }
                    }
                }
            }


            Component {
                id: hslComponent

                Item {
                    GridLayout {
                        anchors.fill: parent
                        columns: 2

                        GameLabel { text: "H"; color: "white"; Layout.preferredWidth: 15 }
                        Item {
                            Layout.fillWidth: true
                            Layout.preferredHeight: root.pickerHeight + app.margins / 2

                            Slider {
                                id: hueSlider
                                anchors.left: parent.left
                                anchors.right: parent.right
                                anchors.verticalCenter: parent.verticalCenter
                                from: 0
                                to: 255
                                stepSize: 1
                                value: color.hslHue * 255
                                onValueChanged: {
                                    if (color.hslHue !== value / 255) {
                                        color.hslHue = value / 255
                                    }
                                }

                                background: LinearGradient {
                                    id: hueGradient
                                    x: hueSlider.leftPadding
                                    width: hueSlider.width - hueSlider.leftPadding - hueSlider.rightPadding
                                    height: root.pickerHeight
                                    anchors.verticalCenter: parent.verticalCenter
                                    start: Qt.point(0, 0)
                                    end: Qt.point(width, 0)
                                    gradient: Gradient {
                                        GradientStop { position: 0.0;  color: "#FF0000" }
                                        GradientStop { position: 0.16; color: "#FFFF00" }
                                        GradientStop { position: 0.33; color: "#00FF00" }
                                        GradientStop { position: 0.5;  color: "#00FFFF" }
                                        GradientStop { position: 0.76; color: "#0000FF" }
                                        GradientStop { position: 0.85; color: "#FF00FF" }
                                        GradientStop { position: 1.0;  color: "#FF0000" }
                                    }
                                }

                                handle: Rectangle {
                                    x: hueSlider.leftPadding + hueSlider.visualPosition * (hueSlider.availableWidth - width)
                                    y: hueSlider.availableHeight / 2 - height / 2
                                    anchors.verticalCenter: parent.verticalCenter
                                    implicitWidth: 5
                                    implicitHeight: root.pickerHeight
                                    color: hueSlider.pressed ? "#f0f0f0" : "#f6f6f6"
                                    border.color: "#bdbebf"
                                }
                            }
                        }


                        GameLabel { text: "S"; color: "white"; Layout.preferredWidth: 15 }
                        Item {
                            Layout.fillWidth: true
                            Layout.preferredHeight: root.pickerHeight + app.margins / 2

                            Slider {
                                id: saturationSlider
                                anchors.left: parent.left
                                anchors.right: parent.right
                                anchors.verticalCenter: parent.verticalCenter
                                from: 0
                                to: 255
                                stepSize: 1
                                value: color.hslSaturation * 255
                                onValueChanged: {
                                    if (color.hslSaturation !== value / 255) {
                                        color.hslSaturation = value / 255
                                    }
                                }
                                background: LinearGradient {
                                    id: saturationGradient
                                    property color startColor: Qt.hsla(color.hslHue, 0, color.hslLightness, 1)
                                    property color stopColor: Qt.hsla(color.hslHue, 1, color.hslLightness, 1)
                                    x: saturationSlider.leftPadding
                                    width: saturationSlider.width - saturationSlider.leftPadding - saturationSlider.rightPadding
                                    height: root.pickerHeight
                                    anchors.verticalCenter: parent.verticalCenter
                                    start: Qt.point(0, 0)
                                    end: Qt.point(width, 0)
                                    gradient: Gradient {
                                        GradientStop { position: 0.0; color: saturationGradient.startColor }
                                        GradientStop { position: 1.0; color: saturationGradient.stopColor }
                                    }
                                }

                                handle: Rectangle {
                                    x: saturationSlider.leftPadding + saturationSlider.visualPosition * (saturationSlider.availableWidth - width)
                                    y: saturationSlider.availableHeight / 2 - height / 2
                                    anchors.verticalCenter: parent.verticalCenter
                                    implicitWidth: 5
                                    implicitHeight: root.pickerHeight
                                    color: saturationSlider.pressed ? "#f0f0f0" : "#f6f6f6"
                                    border.color: "#bdbebf"
                                }
                            }
                        }


                        GameLabel { text: "L"; color: "white"; Layout.preferredWidth: 15 }
                        Item {
                            Layout.fillWidth: true
                            Layout.preferredHeight: root.pickerHeight + app.margins / 2

                            Slider {
                                id: lightnessSlider
                                anchors.left: parent.left
                                anchors.right: parent.right
                                anchors.verticalCenter: parent.verticalCenter
                                from: 0
                                to: 255
                                stepSize: 1
                                value: color.hslLightness * 255
                                onValueChanged: {
                                    if (color.hslLightness !== value / 255) {
                                        color.hslLightness = value / 255
                                    }
                                }
                                background: LinearGradient {
                                    id: lightnessGradient
                                    property color startColor: Qt.hsla(color.hslHue, color.hslSaturation, 0, 1)
                                    property color stopColor: Qt.hsla(color.hslHue, color.hslSaturation, 1, 1)
                                    x: lightnessSlider.leftPadding
                                    width: lightnessSlider.width - lightnessSlider.leftPadding - lightnessSlider.rightPadding
                                    height: root.pickerHeight
                                    anchors.verticalCenter: parent.verticalCenter
                                    start: Qt.point(0, 0)
                                    end: Qt.point(width, 0)
                                    gradient: Gradient {
                                        GradientStop { position: 0.0; color: lightnessGradient.startColor }
                                        GradientStop { position: 1.0; color: lightnessGradient.stopColor }
                                    }
                                }

                                handle: Rectangle {
                                    x: lightnessSlider.leftPadding + lightnessSlider.visualPosition * (lightnessSlider.availableWidth - width)
                                    y: lightnessSlider.availableHeight / 2 - height / 2
                                    anchors.verticalCenter: parent.verticalCenter
                                    implicitWidth: 5
                                    implicitHeight: root.pickerHeight
                                    color: lightnessSlider.pressed ? "#f0f0f0" : "#f6f6f6"
                                    border.color: "#bdbebf"
                                }

                            }
                        }


                        GameLabel { text: "A"; color: "white"; Layout.preferredWidth: 15 }
                        Item {
                            Layout.fillWidth: true
                            Layout.preferredHeight: root.pickerHeight + app.margins / 2

                            Slider {
                                id: alphaSlider
                                anchors.left: parent.left
                                anchors.right: parent.right
                                anchors.verticalCenter: parent.verticalCenter
                                from: 0
                                to: 255
                                stepSize: 1
                                value: color.a * 255
                                onValueChanged: {
                                    if (color.a !== value / 255) {
                                        color.a = value / 255
                                    }
                                }
                                background: LinearGradient {
                                    id: alphaGradient
                                    property color startColor: Qt.hsla(color.hslHue, color.hslSaturation, color.hslLightness, 0)
                                    property color stopColor: Qt.hsla(color.hslHue, color.hslSaturation, color.hslLightness, 1)
                                    x: alphaSlider.leftPadding
                                    width: alphaSlider.width - alphaSlider.leftPadding - alphaSlider.rightPadding
                                    height: root.pickerHeight
                                    anchors.verticalCenter: parent.verticalCenter
                                    start: Qt.point(0, 0)
                                    end: Qt.point(width, 0)
                                    gradient: Gradient {
                                        GradientStop { position: 0.0; color: alphaGradient.startColor }
                                        GradientStop { position: 1.0; color: alphaGradient.stopColor }
                                    }
                                }

                                handle: Rectangle {
                                    x: alphaSlider.leftPadding + alphaSlider.visualPosition * (alphaSlider.availableWidth - width)
                                    y: alphaSlider.availableHeight / 2 - height / 2
                                    anchors.verticalCenter: parent.verticalCenter
                                    implicitWidth: 5
                                    implicitHeight: root.pickerHeight
                                    color: alphaSlider.pressed ? "#f0f0f0" : "#f6f6f6"
                                    border.color: "#bdbebf"
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
