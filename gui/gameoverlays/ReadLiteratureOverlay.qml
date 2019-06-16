import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5

import Chronicles 1.0

import "../components"

GameOverlayItem {
    id: root

    property LiteratureItem literatureItem: Game.engine.currentLiteratureItem

    Item {
        anchors.fill: parent
        anchors.margins: app.margins

        Rectangle {
            anchors.fill: parent
            color: "black"
            opacity: 0.4
            radius: app.gridSize
        }

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: app.margins

            GameLabel {
                id: bookTitle
                Layout.alignment: Qt.AlignHCenter
                color: "white"
                font.pixelSize: app.largeFont
                text: literatureItem ? literatureItem.title : ""
            }

            ScrollView {
                id: textView
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.margins: app.margins
                clip: true

                TextArea {
                    id: bookTextArea
                    wrapMode: TextEdit.WordWrap
                    readOnly: true
                    color: "white"
                    font.family: app.fontFamily
                    font.pixelSize: app.mediumFont
                    text: literatureItem ? literatureItem.text : ""
                }
            }

            GameButton {
                Layout.fillWidth: true
                text: qsTr("Close")
                onClicked: Game.engine.finishReading()
            }
        }
    }

}
