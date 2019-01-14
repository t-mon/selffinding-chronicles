import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12

Item {
    id: root

    property real percentage
    property color color

    Behavior on opacity { NumberAnimation { duration: 200 } }

    Row {
        id: indicatorRow
        anchors.centerIn: root

        Rectangle {
            color: root.color
            height: root.height
            width: root.width * root.percentage / 100.0
            Behavior on width { NumberAnimation { duration: 400 } }
        }

        Rectangle {
            color: "gray"
            height: root.height
            width: root.width * (1 - root.percentage / 100.0)
            Behavior on width { NumberAnimation { duration: 400 } }
        }
    }


}
