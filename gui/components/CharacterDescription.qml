import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0

import Chronicles 1.0

Item {
    id: root

    property Character character

    Rectangle {
        id: backgroundRectangle
        anchors.fill: parent
        color: "#55000000"
        border.color: "white"
        border.width: app.borderWidth
    }

    ColumnLayout {
        id: playerPropertyLayout
        spacing: app.margins / 2
        anchors.fill: parent
        anchors.margins: app.margins

        GameLabel {
            Layout.alignment: Qt.AlignHCenter
            text: character ? character.name : ""
            font.pixelSize: largeFont
            color: "white"
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: app.borderWidth / 2
            color: "white"
        }

        RowLayout {
            Layout.fillWidth: true

            GameLabel {
                Layout.fillWidth: true
                color: "white"
                text: qsTr("Experience")
            }

            GameLabel {
                color: "white"
                text: character ? character.experience : ""
            }
        }


        RowLayout {
            Layout.fillWidth: true

            GameLabel {
                Layout.fillWidth: true
                color: "white"
                text: qsTr("Health")
            }

            GameLabel {
                color: "white"
                text: character ? character.health  + "/" + character.healthMax : ""
            }
        }

        RowLayout {
            Layout.fillWidth: true

            GameLabel {
                Layout.fillWidth: true
                color: "white"
                text: qsTr("Mana")
            }

            GameLabel {
                color: "white"
                text: character ? character.mana  + "/" + character.manaMax : ""
            }
        }

        RowLayout {
            Layout.fillWidth: true

            GameLabel {
                Layout.fillWidth: true
                color: "white"
                text: qsTr("Wisdom")
            }

            GameLabel {
                color: "white"
                text: character ? character.wisdom : ""
            }
        }

        RowLayout {
            Layout.fillWidth: true

            GameLabel {
                Layout.fillWidth: true
                color: "white"
                text: qsTr("Strength")
            }

            GameLabel {
                color: "white"
                text: character ? character.strength : ""
            }
        }

        RowLayout {
            Layout.fillWidth: true

            GameLabel {
                Layout.fillWidth: true
                color: "white"
                text: qsTr("Stealth")
            }

            GameLabel {
                color: "white"
                text: character ? character.stealth : ""
            }
        }
    }
}
