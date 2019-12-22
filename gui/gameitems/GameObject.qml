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

    property GameObject gameObject
    antialiasing: app.antialiasing
    property real angle: 0

    onXChanged: {
        if (!gameObject) return
        gameObject.position.x = x / app.gridSize
    }

    onYChanged: {
        if (!gameObject) return
        gameObject.position.y = y / app.gridSize
    }

    linearDamping: 30
    fixedRotation: true

    Component.onCompleted: {
        if (!gameObject) return

        root.bodyType = gameObject.bodyType
        switch (gameObject.shape) {
        case GameItem.ShapeCircle:
            root.body.addFixture(circleFixtureComponent.createObject(root, { parentItem: root, gameObject: root.gameObject }))
            root.body.addFixture(circleSensorFixtureComponent.createObject(root, { parentItem: root, gameObject: root.gameObject }))
            break;
        case GameItem.ShapeRectangle:
            root.body.addFixture(rectangleFixtureComponent.createObject(root, { parentItem: root, gameObject: root.gameObject }))
            root.body.addFixture(rectangleSensorFixtureComponent.createObject(root, { parentItem: root, gameObject: root.gameObject }))
            break;
        default:
            break;
        }
    }

    transform: Rotation {
        origin {
            x: gameObject ? gameObject.physicsPosition.x * app.gridSize + gameObject.physicsSize.width * app.gridSize / 2 : 0
            y: gameObject ? gameObject.physicsPosition.y * app.gridSize + gameObject.physicsSize.height * app.gridSize / 2 : 0
        }
        angle: root.angle
        axis { x: 0; y: 0; z: 1 }
    }

    Component {
        id: circleFixtureComponent

        Circle {
            id: circleFixture
            property PhysicsItem parentItem
            property GameObject gameObject

            radius: gameObject.physicsSize.width / 2 * app.gridSize
            x: gameObject.physicsPosition.x * app.gridSize
            y: gameObject.physicsPosition.y * app.gridSize
            density: 1
            friction: 0
            restitution: 0.0
            categories: gameObject.categoryFlag
            collidesWith: gameObject.collisionFlag
        }
    }

    Component {
        id: circleSensorFixtureComponent

        Circle {
            id: circleSensorFixture
            property PhysicsItem parentItem
            property GameObject gameObject
            sensor:  true
            radius: gameObject.physicsSize.width / 2 * app.gridSize
            x: gameObject.physicsPosition.x * app.gridSize
            y: gameObject.physicsPosition.y * app.gridSize
            density: 0
            friction: 0
            restitution: 0
            categories: GameObject.PhysicsSensor
        }
    }

    Component {
        id: rectangleFixtureComponent

        Box {
            id: rectangleFixture
            property PhysicsItem parentItem
            property GameObject gameObject

            width: gameObject.physicsSize.width * app.gridSize
            height: gameObject.physicsSize.height * app.gridSize
            x: gameObject.physicsPosition.x * app.gridSize
            y: gameObject.physicsPosition.y * app.gridSize
            density: 1
            friction: 0
            restitution: 0.0
            categories: gameObject.categoryFlag
            collidesWith: gameObject.collisionFlag
        }
    }

    Component {
        id: rectangleSensorFixtureComponent

        Box {
            id: rectangleSensorFixture
            property PhysicsItem parentItem
            property GameObject gameObject

            sensor:  true
            width: gameObject.physicsSize.width * app.gridSize
            height: gameObject.physicsSize.height * app.gridSize
            x: gameObject.physicsPosition.x * app.gridSize
            y: gameObject.physicsPosition.y * app.gridSize
            density: 0
            friction: 0
            restitution: 0
            categories: GameObject.PhysicsSensor
        }
    }

    // TODO: vertices fixture


    Image {
        id: objectImage
        anchors.fill: parent
        source: gameObject ? dataDirectory + gameObject.imageName : ""
        opacity: root.itemDebugEnabled ? 0.5 : 1
    }

    Loader {
        anchors.fill: parent
        active: root.itemDebugEnabled
        source: "../components/ItemDebugFrame.qml"
    }
}

