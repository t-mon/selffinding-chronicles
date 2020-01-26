import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtQuick.Controls.Material 2.0

Item {
    id: root
    property bool toggleButton: false
    property bool enabled: false
    property bool selected: false
    property color baseColor: "gray"

    property string title
    property string iconSource

    signal clicked()

    Rectangle {
        anchors.fill: parent
        radius: height / 4
        color: {
            if (root.toggleButton) {
                if (root.enabled || root.selected) {
                    return "green"
                } else {
                    return "gray"
                }
            } else {
                if (root.selected) {
                    return "green"
                } else {
                    return root.baseColor
                }
            }
        }
    }

    Image {
        id:  toolImage
        anchors.centerIn: parent
        width: parent.width * 0.7
        height: width
        source: iconSource
    }

    MouseArea {
        id: toolMouseArea
        anchors.fill: parent
        hoverEnabled: true
        onClicked: {
            if (root.toggleButton) {
                root.enabled = !root.enabled
            } else {
                root.clicked()
            }
        }


        ToolTip {
            parent: toolMouseArea.handle
            visible: toolMouseArea.containsMouse
            text: root.title
            timeout: 2000
            delay: 250
        }
    }
}
