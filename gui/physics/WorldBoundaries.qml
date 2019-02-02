import QtQuick 2.0
import Box2D 2.0

Body {
    id: root
    world: physicsWorld

    property var worldObject

    Box {
        y: worldObject.height
        width: worldObject.width
        height: app.gridSize
    }

    Box {
        y: -app.gridSize
        height: app.gridSize
        width: worldObject.width
    }

    Box {
        x: -app.gridSize
        width: app.gridSize
        height: worldObject.height
    }

    Box {
        x: worldObject.width
        width: app.gridSize
        height: worldObject.height
    }
}
