import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0

import Chronicles 1.0

Item {
    id: root

    property Character character

    Rectangle {
        id: backgroundRectangle
        anchors.fill: parent
        color: "transparent"
        border.color: "white"
        border.width: app.borderWidth
    }

    Column {
        id: playerPropertyLayout
        spacing: app.margins / 2
        anchors.fill: parent
        anchors.margins: app.margins

        GameLabel {
            color: "white"
            text: qsTr("Experience") + ": " + character.experience
        }

        GameLabel {
            color: "white"
            text: qsTr("Health") + ": " + character.health  + "/" + character.healthMax
        }

        GameLabel {
            color: "white"
            text: qsTr("Mana") + ": " + character.mana  + "/" + character.manaMax
        }

        GameLabel {
            color: "white"
            text: qsTr("Wisdom") + ": " + character.wisdom
        }

        GameLabel {
            color: "white"
            text: qsTr("Strength") + ": " + character.strength
        }

        GameLabel {
            color: "white"
            text: qsTr("Stealth") + ": " + character.stealth
        }
    }
}
