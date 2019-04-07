import QtQuick 2.0
import Box2D 2.0

Body {
    id: root
    world: physicsWorld

    property var worldObject
    property real borderWidth: 1

    Box {
        y: worldObject.height
        width: worldObject.width
        height: borderWidth
    }

    Box {
        y: -borderWidth
        height: borderWidth
        width: worldObject.width
    }

    Box {
        x: -borderWidth
        width: borderWidth
        height: worldObject.height
    }

    Box {
        x: worldObject.width
        width: borderWidth
        height: worldObject.height
    }
}
