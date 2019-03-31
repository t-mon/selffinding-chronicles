import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12

import Chronicles 1.0
import "../components"

GamePage {
    id: root

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: app.margins * 8

        spacing: app.margins


        RowLayout {
            GameLabel {
                text: qsTr("Map name")
                color: "white"
            }

            TextField {
                id: mapNameField
                text: qsTr("Map name")
                Layout.fillWidth: true
            }
        }

        RowLayout {
            GameLabel {
                text: qsTr("Map width")
                color: "white"
            }

            SpinBox {
                id: mapWidthSponBox
                value: 100
                from: 50
                to: 5000
                editable: true
                Layout.fillWidth: true
            }
        }

        RowLayout {
            GameLabel {
                text: qsTr("Map height")
                color: "white"
            }

            SpinBox {
                id: mapHeightSponBox
                value: 100
                from: 50
                to: 5000
                editable: true
                Layout.fillWidth: true
            }
        }

        Item {
            id: spacingItem
            Layout.fillWidth: true
            Layout.fillHeight: true
        }

        RowLayout {
            Layout.fillWidth: true

            GameButton {
                Layout.fillWidth: true
                text: qsTr("Cancel")
                font.pixelSize: app.largeFont
                onClicked: pageStack.pop()
            }

            GameButton {
                Layout.fillWidth: true
                text: qsTr("Create")
                font.pixelSize: app.largeFont
                onClicked: {
                    console.log("Create map", mapNameField.text)
                    Game.mapEditor.createNewMap()
                    Game.mapEditor.map.name = mapNameField.text;
                    Game.mapEditor.map.size = Qt.size(mapWidthSponBox.value, mapHeightSponBox.value)

                    pageStack.push(Qt.resolvedUrl("MapEditor.qml"))
                }
            }
        }
    }
}
