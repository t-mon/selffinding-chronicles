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

    onPlayerAuraRangeChanged: gameItem.playerVisible = playerAuraRange
    onPlayerOnItemChanged: {
        if (!gameItem) return
        gameItem.playerOnItem = playerOnItem
    }

    onXChanged: {
        if (!gameItem) return
        gameItem.position.x = x / app.gridSize
    }

    onYChanged: {
        if (!gameItem) return
        gameItem.position.y = y / app.gridSize
        z = worldItem.calculateLayerValue(gameItem.layer, y, height, worldItem.height)
    }

    linearDamping: 30
    fixedRotation: true

    Component.onCompleted: {
        if (!gameItem) return

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

    // TODO: vertices fixture




    Image {
        id: itemImage
        anchors.fill: parent
        source: gameItem ? dataDirectory + gameItem.imageName : ""
        opacity: root.itemDebugEnabled ? 0.5 : 1
    }

//    // Default shader
//    ShaderEffect {
//        id: shadowShader
//        width: parent.width
//        height: parent.height
//        blending: false

//        fragmentShader: "
//            uniform lowp float qt_Opacity;
//            uniform highp vec2 offset;
//            uniform sampler2D source;
//            uniform sampler2D shadow;
//            uniform highp float darkness;
//            uniform highp vec2 delta;

//            varying highp vec2 qt_TexCoord0;

//            void main() {
//                lowp vec4 fg = texture2D(source, qt_TexCoord0);
//                lowp vec4 bg = texture2D(shadow, qt_TexCoord0 + delta);
//                gl_FragColor = (fg + vec4(0., 0., 0., darkness * bg.a) * (1. - fg.a)) * qt_Opacity;
//            }
//        "
//    }

    Glow {
        id: glowEffect
        source: itemImage
        anchors.fill: itemImage
        radius: 8
        samples: 20
        color: "white"
        visible: gameItem && gameItem.playerFocus ? true : false
        opacity: visible ? 1 : 0
        Behavior on opacity { NumberAnimation { duration: 200 } }
    }

    ItemDescription {
        id: nameLabel
        anchors.bottom: root.top
        anchors.horizontalCenter: root.horizontalCenter
        text: gameItem ? gameItem.name : ""
        opacity: gameItem ? (root.itemDebugEnabled ? 0.5 : (gameItem.playerFocus ? 1 : 0)) : 0
    }

    Loader {
        anchors.fill: parent
        active: root.itemDebugEnabled
        source: "../components/ItemDebugFrame.qml"
    }
}

