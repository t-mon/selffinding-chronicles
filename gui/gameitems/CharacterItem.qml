import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0

import Box2D 2.0
import Chronicles 1.0
import "../components"
import "../physics"

PhysicsItem {
    id: root

    /*
      Box.Category1 = character
      Box.Category2 = item which is not an obstacle
      Box.Category3 = item which is an obstacle
     */

    property Character character: null
    property bool isPlayer: false
    property int itemType: character.itemType

    property real auraRadius: (character.auraRange + character.size.width / 2) * app.gridSize
    property real attackRadius: character.size.width / 2 * app.gridSize

    antialiasing: app.antialiasing
    bodyType: character.movable ? Body.Dynamic : Body.Static
    //rotation: character.angle * 180 / Math.PI

//    Connections {
//        target: root.character
//        onAngleChanged: {
//            body.t
//        }
//    }

    fixtures: [
        Circle {
            id: bodyCircle
            radius: root.width / 2
            density: 100
            friction: 0.0
            restitution: 0.0
            categories: Box.Category1
            collidesWith: Box.Category1 | Box.Category3
        },
        Polygon {
            id: auraSensor
            sensor: true
            vertices: [
                Qt.point(root.width / 2, root.height / 2),
                Qt.point(root.width / 2 + auraRadius * Math.cos(Math.PI / 4), root.width / 2 + auraRadius * Math.sin(Math.PI / 4)),
                Qt.point(root.width / 2 + auraRadius * Math.cos(Math.PI / 8), root.width / 2 + auraRadius * Math.sin(Math.PI / 8)),
                Qt.point(root.width / 2 + auraRadius * Math.cos(0), root.width / 2 + auraRadius * Math.sin(0)),
                Qt.point(root.width / 2 + auraRadius * Math.cos(-Math.PI / 8), root.width / 2 + auraRadius * Math.sin(-Math.PI / 8)),
                Qt.point(root.width / 2 + auraRadius * Math.cos(-Math.PI / 4), root.width / 2 + auraRadius * Math.sin(-Math.PI / 4))
            ]

            onBeginContact: {
                if (!isPlayer)
                    return

                var target = other.getBody().target
                if (target.itemType) {
                    //console.log("---------> Begin contact " + other + " | " + target.itemType)
                    target.playerAuraRange = true
                }
            }

            onEndContact: {
                if (!isPlayer)
                    return

                var target = other.getBody().target
                if (target.itemType) {
                    //console.log("---------> End contact " + other + " | " + target.itemType)
                    target.playerAuraRange = false
                }
            }
        }

        //        Circle {
        //            id: auraSensor
        //            radius: auraRadius
        //            sensor: true
        //            density: 0
        //            friction: 0
        //            restitution: 0
        //            x: -radius + root.width / 2
        //            y: -radius + root.height / 2

        //            onBeginContact: {
        //                if (!isPlayer)
        //                    return

        //                var target = other.getBody().target
        //                if (target.itemType) {
        //                    //console.log("---------> Begin contact " + other + " | " + target.itemType)
        //                    target.playerAuraRange = true
        //                }
        //            }

        //            onEndContact: {
        //                if (!isPlayer)
        //                    return

        //                var target = other.getBody().target
        //                if (target.itemType) {
        //                    //console.log("---------> End contact " + other + " | " + target.itemType)
        //                    target.playerAuraRange = false
        //                }
        //            }
        //        }


        /*,
        Polygon {
            id: damageSensor
            sensor: true
            vertices: [
                Qt.point(root.width / 2, root.height / 2),
                Qt.point(root.width / 2 + attackRadius * Math.cos(Math.PI / 4), root.width / 2 + attackRadius * Math.sin(Math.PI / 4)),
                Qt.point(root.width / 2 + attackRadius * Math.cos(Math.PI / 8), root.width / 2 + attackRadius * Math.sin(Math.PI / 8)),
                Qt.point(root.width / 2 + attackRadius * Math.cos(0), root.width / 2 + attackRadius * Math.sin(0)),
                Qt.point(root.width / 2 + attackRadius * Math.cos(-Math.PI / 8), root.width / 2 + attackRadius * Math.sin(-Math.PI / 8)),
                Qt.point(root.width / 2 + attackRadius * Math.cos(-Math.PI / 4), root.width / 2 + attackRadius * Math.sin(-Math.PI / 4))
            ]

            onBeginContact: {
                var target = other.getBody().target
                //getBody().beginContact(other)
                console.log("Damage begin contact " + target.character.name)
            }
            onEndContact: {
                //other.getBody().endContact()
                //console.log("Damage end contact")
            }
        }*/
    ]



    //    PropertyAnimation {
    //        id: attackAnimation
    //        target: root
    //        running: true
    //        loops: Animation.Infinite
    //        property: "attackRadius"
    //        duration: 1000
    //        from: character.size.width / 2 * app.gridSize
    //        to: character.size.width / 2 * app.gridSize + 2 * app.gridSize
    //    }

    ItemDescription {
        id: nameLabel
        anchors.bottom: root.top
        anchors.horizontalCenter: root.horizontalCenter
        text: character ? character.name : ""
        opacity: character ? (Game.debugging ? 0.5 : (root.playerAuraRange && !root.isPlayer ? 1 : 0)) : 0
    }

    Image {
        id: playerImage
        anchors.fill: parent
        source: character.heading === Character.HeadingRight ? dataDirectory + "/images/characters/player-male.png" : dataDirectory + "/images/characters/player-male-mirror.png"
        opacity: Game.debugging ? 0.5 : 1
    }

    //    Item {
    //        anchors.fill: parent
    //        rotation: character.angle * 180 / Math.PI + 90
    //        antialiasing: app.antialiasing

    //        Rectangle {
    //            id: wireFrame
    //            anchors.fill: parent
    //            color: "transparent"
    //            border.color: "red"
    //            opacity: 0.3
    //            border.width: 2
    //            visible: Game.debugging
    //            radius: Game.world.player.shape === GameObject.ShapeCircle ? width / 2 : 0
    //        }

    //        Item {
    //            id: auraItem
    //            width: auraRadius * 2
    //            height: auraRadius * 2
    //            anchors.centerIn: parent
    //            z: Map.Layer4Highest
    //            opacity: Game.debugging ? 0.5 : 0

    //            Image {
    //                id: auraImage
    //                anchors.fill: parent
    //                source: dataDirectory + "/images/characters/player-aura.png"
    //            }
    //        }
    //    }
}
