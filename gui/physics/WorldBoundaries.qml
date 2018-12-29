import QtQuick 2.0
import Box2D 2.0

Body {
    id: root
    world: physicsWorld

    property var worldObject

    Box {
        y: root.worldObject.height
        width: root.worldObject.width
        height: app.gridSize
    }

    Box {
        y: -app.gridSize
        height: app.gridSize
        width: root.worldObject.width
    }

    Box {
        x: -app.gridSize
        width: app.gridSize
        height: root.worldObject.height
    }

    Box {
        x: root.worldObject.width
        width: app.gridSize
        height: root.worldObject.height
    }
}
