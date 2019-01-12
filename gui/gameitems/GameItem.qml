import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0

import Box2D 2.0
import Chronicles 1.0

import "../components"
import "../physics"

PhysicsItem {
    id: root

    property GameItem gameItem: Game.world.gameItems.get(model.index)
    property int itemType: gameItem ? gameItem.itemType : GameItem.TypeNone

    antialiasing: app.antialiasing
    opacity: gameItem ? (gameItem.hidingPlayer ? 0.5 : 1) : 0

    onPlayerAuraRangeChanged: gameItem.playerVisible = playerAuraRange
    onPlayerOnItemChanged: gameItem.playerOnItem = playerOnItem

    linearDamping: 2
    fixedRotation: true

    Component.onCompleted: {
        if (!gameItem)
            return

        root.bodyType = gameItem.bodyType
        switch (gameItem.shape) {
        case GameItem.ShapeCircle:
            root.body.addFixture(circleFixtureComponent.createObject(root, { parentItem: root, gameItem: root.gameItem }))
            root.body.addFixture(circleSensorFixtureComponent.createObject(root, { parentItem: root, gameItem: root.gameItem }))
            break;
        default:
            break;
        }
    }


    Component {
        id: circleFixtureComponent
        Circle {
            id: circleFixture

            property PhysicsItem parentItem
            property GameItem gameItem

            radius: gameItem.physicsSize.width / 2 * app.gridSize
            x: gameItem.physicsPosition.x * app.gridSize
            y: gameItem.physicsPosition.y * app.gridSize
            density: 1
            friction: 0
            restitution: 0.0
            categories: gameItem.categoryFlag
            collidesWith: gameItem.collisionFlag
        }
    }

    Component {
        id: circleSensorFixtureComponent
        Circle {
            id: circleSensorFixture

            property PhysicsItem parentItem
            property GameItem gameItem
            sensor:  true
            radius: gameItem.physicsSize.width / 2 * app.gridSize
            x: gameItem.physicsPosition.x * app.gridSize
            y: gameItem.physicsPosition.y * app.gridSize
            density: 0
            friction: 0
            restitution: 0
            categories: GameItem.PhysicsSensor
        }
    }

    Connections {
        target: root.gameItem
        onPlayerOnItemChanged: {
            console.log("Player on item " + playerOnItem)
            if (root.gameItem.itemType === GameItem.TypeTree && playerOnItem) {
                console.log("Start animation")
                playerOnItemAnimation.start()
            }
        }
    }

    SequentialAnimation {
        id: playerOnItemAnimation
        loops: 1

        RotationAnimation {
            target: root
            property: "rotation"
            to: 3
            duration: 80
        }

        RotationAnimation {
            target: root
            property: "rotation"
            to: 0
            duration: 80
        }

        RotationAnimation {
            target: root
            property: "rotation"
            to: -3
            duration: 80
        }

        RotationAnimation {
            target: root
            property: "rotation"
            to: 0
            duration: 80
        }
    }

    Image {
        id: itemImage
        anchors.fill: parent
        source: gameItem ? dataDirectory + gameItem.imageName : ""
        opacity: Game.debugging ? 0.5 : 1
    }

    ItemDescription {
        id: nameLabel
        anchors.bottom: root.top
        anchors.horizontalCenter: root.horizontalCenter
        text: gameItem ? gameItem.name : ""
        opacity: gameItem ? (Game.debugging ? 0.5 : (gameItem.playerFocus ? 1 : 0)) : 0
    }

    //    Component.onCompleted: console.log("Created game item " + gameItem.name + " " + gameItem.imageName)
    //    Component.onDestruction: console.log("Destroy item")

}

