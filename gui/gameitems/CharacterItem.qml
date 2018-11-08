import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0

import Chronicles 1.0
import "../components"


Item {
    id: root

    property Character character: null

    ItemDescription {
        id: nameLabel
        anchors.bottom: root.top
        anchors.horizontalCenter: root.horizontalCenter
        text: character ? character.name : ""
        opacity: character ? (Game.debugging ? 0.5 : (character.playerFocus ? 1 : 0)) : 0
    }

    Image {
        id: playerImage
        anchors.fill: parent
        source: character.heading === Character.HeadingRight ? dataDirectory + "/images/characters/player-male.png" : dataDirectory + "/images/characters/player-male-mirror.png"
        opacity: Game.debugging ? 0.5 : 1
    }

    Item {
        anchors.fill: parent
        rotation: character.angle * 180 / Math.PI + 90
        antialiasing: app.antialiasing

        Rectangle {
            id: wireFrame
            anchors.fill: parent
            color: "transparent"
            border.color: "red"
            opacity: 0.3
            border.width: 2
            visible: Game.debugging
            radius: Game.world.player.shape === GameObject.ShapeCircle ? width / 2 : 0
        }

        Item {
            id: auraItem
            width: character.auraCircleObject.size.width * app.gridSize
            height: character.auraCircleObject.size.height * app.gridSize
            anchors.centerIn: parent
            z: character.auraCircleObject.layer

            opacity: Game.debugging ? 0.5 : 0

            Image {
                id: auraImage
                anchors.fill: parent
                source: dataDirectory + "/images/characters/player-aura.png"
            }
        }
    }


}
