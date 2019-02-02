import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0

TextField {
    id: root
    font: app.fontFamily

    background: Rectangle {
        implicitWidth: 200
        implicitHeight: 40
        color: root.enabled ? "transparent" : "#353637"
        border.color: root.enabled ? "#21be2b" : "transparent"
    }
}
