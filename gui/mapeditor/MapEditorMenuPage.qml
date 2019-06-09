import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0

import Chronicles 1.0
import "../components"

GamePage {
    id: root

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: app.margins * 8

        spacing: app.margins

        Button {
            Layout.fillWidth: true
            text: qsTr("Create new map")
            font.pixelSize: app.largeFont
            onClicked: {
                pageStack.push(Qt.resolvedUrl("CreateNewMapPage.qml"))
            }
        }

        Button {
            Layout.fillWidth: true
            text: qsTr("Load map")
            font.pixelSize: app.largeFont
            //onClicked: Game.engine.resumeGame()
        }

        Item {
            id: spacingItem
            Layout.fillWidth: true
            Layout.fillHeight: true
        }

        Button {
            Layout.fillWidth: true
            text: qsTr("Cancel")
            font.pixelSize: app.largeFont
            onClicked: pageStack.pop()
        }
    }
}
