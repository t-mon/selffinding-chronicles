import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0

import Chronicles 1.0

Page {
    id: root

    ColumnLayout {

        anchors.centerIn: parent

        Button {
            id: playButton
            Layout.alignment: Qt.AlignHCenter
            text: "Play"
            onClicked: pageStack.push(Qt.resolvedUrl("GamePage.qml"))
        }

        Button {
            id: settingsButton
            Layout.alignment: Qt.AlignHCenter
            text: "Settings"
            onClicked: pageStack.push(Qt.resolvedUrl("SettingsPage.qml"))
        }

        Button {
            id: aboutButton
            Layout.alignment: Qt.AlignHCenter
            text: "About"
            onClicked: pageStack.push(Qt.resolvedUrl("AboutPage.qml"))
        }
    }



}
