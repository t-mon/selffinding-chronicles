import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.0

import Box2D 2.0
import Chronicles 1.0

import "../components"
import "../physics"

PhysicsItem {
    id: root

    property GameItem gameItem
    property int itemType: gameItem ? gameItem.itemType : GameItem.TypeNone
    property real angle: 0

    antialiasing: app.antialiasing
    opacity: gameItem ? (gameItem.hidingPlayer ? 0.5 : 1) : 0

    onPlayerAuraRangeChanged: gameItem.playerVisible = playerAuraRange
    onPlayerOnItemChanged: {
        //console.log("Player on item", playerOnItem)
        gameItem.playerOnItem = playerOnItem
    }

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
        case GameItem.ShapeRectangle:
            root.body.addFixture(rectangleFixtureComponent.createObject(root, { parentItem: root, gameItem: root.gameItem }))
            root.body.addFixture(rectangleSensorFixtureComponent.createObject(root, { parentItem: root, gameItem: root.gameItem }))
            break;
        default:
            break;
        }
    }

    transform: Rotation {
        origin {
            x: gameItem ? gameItem.physicsPosition.x * app.gridSize + gameItem.physicsSize.width * app.gridSize / 2 : 0
            y: gameItem ? gameItem.physicsPosition.y * app.gridSize + gameItem.physicsSize.height * app.gridSize / 2 : 0
        }
        angle: root.angle
        axis { x: 0; y: 0; z: 1 }
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

    Component {
        id: rectangleFixtureComponent
        Box {
            id: rectangleFixture

            property PhysicsItem parentItem
            property GameItem gameItem

            width: gameItem.physicsSize.width * app.gridSize
            height: gameItem.physicsSize.height * app.gridSize
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
        id: rectangleSensorFixtureComponent
        Box {
            id: rectangleSensorFixture

            property PhysicsItem parentItem
            property GameItem gameItem

            sensor:  true
            width: gameItem.physicsSize.width * app.gridSize
            height: gameItem.physicsSize.height * app.gridSize
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
            //console.log("Player on item " + playerOnItem)
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
            property: "angle"
            to: 2
            duration: 80
        }

        RotationAnimation {
            target: root
            property: "angle"
            to: 0
            duration: 80
        }

        RotationAnimation {
            target: root
            property: "rotation"
            to: -2
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
        opacity: root.itemDebugEnabled ? 0.5 : 1
    }

    ItemDescription {
        id: nameLabel
        anchors.bottom: root.top
        anchors.horizontalCenter: root.horizontalCenter
        text: gameItem ? gameItem.name : ""
        opacity: gameItem ? (root.itemDebugEnabled ? 0.5 : (gameItem.playerFocus ? 1 : 0)) : 0
    }

    Rectangle {
        id: wireFrame
        anchors.fill: parent
        color: "gray";
        border.color: "white";
        border.width: app.borderWidth / 2
        opacity: root.itemDebugEnabled ? 0.2 : 0
    }

//    FlameItem {
//        id: fireItem
//        enabled: true
//        anchors.centerIn: parent
//        turbulence: debugControls.turbulenceEnabled
//        particleSystem: particles
//        width: app.gridSize * 6
//        height: app.gridSize * 6
//        angle: 270
//        angleVariation: 30
//        magnitude: 30
//    }

    //    Component.onCompleted: console.log("Created game item " + gameItem.name + " " + gameItem.imageName)
    //    Component.onDestruction: console.log("Destroy item")

}

