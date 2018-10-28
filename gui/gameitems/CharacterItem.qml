import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0

import Chronicles 1.0
import "../components"


Item {
    id: root

    property var characterItem: Game.world.characterItems.get(model.index)

    GameLabel {
        id: nameLabel
        anchors.bottom: root.top
        anchors.horizontalCenter: root.horizontalCenter
        text: characterItem ? characterItem.name : ""
        color: "black"
        visible: characterItem ? characterItem.playerFocus || Game.debugging : Game.debugging
    }

    Item {
        anchors.fill: parent
        rotation: characterItem.angle * 180 / Math.PI + 90
        antialiasing: app.antialiasing

        Image {
            id: playerImage
            anchors.fill: parent
            source: dataDirectory + "/images/characters/player.png"
            opacity: Game.debugging ? 0.5 : 1
        }

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
            width: characterItem.auraCircleObject.size.width * app.gridSize
            height: characterItem.auraCircleObject.size.height * app.gridSize
            anchors.centerIn: parent
            z: characterItem.auraCircleObject.layer

            opacity: Game.debugging ? 0.5 : 0

            Image {
                id: auraImage
                anchors.fill: parent
                source: dataDirectory + "/images/characters/player-aura.png"
            }
        }
    }


}
