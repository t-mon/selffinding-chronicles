import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0

import Box2D 2.0
import Chronicles 1.0

import "../components"
import "../physics"

PhysicsItem {
    id: root

    //property Bullet bulletObject

    fixedRotation: true

    Component.onCompleted: console.log("Created bullet")

    fixtures: [
        Circle {
            id: bullet
            radius: root.width / 2
            density: 1
            friction: 0.0
            restitution: 0.0
            categories: GameObject.PhysicsBullet
            collidesWith: GameObject.PhysicsAll
            bullet: true

            onBeginContact: {
                if (!isPlayer)
                    return

                var target = other.getBody().target
                if (target.itemType) {
                    console.log("## Bullet hit!!")
                    //target.playerOnItem = true
                }
            }

            onEndContact: {
                if (!isPlayer)
                    return

                var target = other.getBody().target
                if (target.itemType) {
                    //target.playerOnItem = false
                }
            }
        }
    ]

    Rectangle {
        anchors.fill: parent
        radius: width / 2
        color: "red"
    }

//    Image {
//        id: arrowImage
//        anchors.fill: root
//        source: "/images/items/arrow.png"
//    }

}
