import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0

import Chronicles 1.0
import "components"

Page {
    id: root

    header: ChroniclesHeader {
        text: qsTr("Settings")
        backButtonVisible: true
        onBackPressed: pageStack.pop()
    }

    Flickable {
        anchors.fill: parent
        contentHeight: settingsColumn.implicitHeight
        anchors.leftMargin: app.margins
        anchors.rightMargin: app.margins


        ColumnLayout {
            id: settingsColumn
            width: parent.width

            RowLayout {
                Layout.fillWidth: true

                spacing: app.margins

                Label {
                    text: qsTr("Controll mode")
                }

                ComboBox {
                    Layout.fillWidth: true
                    model: [qsTr("Keyboard"), qsTr("Keyboard + Mouse"), qsTr("Touchscreen")]
                    onCurrentIndexChanged: {
                        if (currentIndex == 0) {
                            Game.controlMode = Game.ControlModeKeyBoard
                        } else if (currentIndex == 1) {
                            Game.controlMode = Game.ControlModeKeyBoardMouse
                        } else if (currentIndex == 2) {
                            Game.controlMode = Game.ControlModeTouchscreen
                        }
                    }
                }

            }


        }
    }
}
