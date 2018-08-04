import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0

import Chronicles 1.0

import "components"

Page {
    id: root

    header: ChroniclesHeader {
        text: qsTr("About %1").arg(app.title)
        backButtonVisible: true
        onBackPressed: pageStack.pop()
    }

    Flickable {
        anchors.fill: parent
        contentHeight: settingsColumn.implicitHeight
        anchors.margins: app.margins

        ColumnLayout {
            id: settingsColumn
            width: parent.width

            RowLayout {
                Layout.fillWidth: true

                spacing: app.margins

                GameLabel {
                    Layout.fillWidth: true
                    font.pixelSize: app.largeFont
                    text: qsTr("Developer")
                }

                GameLabel {
                    Layout.fillWidth: true
                    font.pixelSize: app.largeFont
                    text: qsTr("Simon Stürz")
                }
            }
        }
    }
}
