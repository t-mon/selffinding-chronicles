import QtQuick 2.9
import Box2D 2.0
import Chronicles 1.0

Circle {
    id: root

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
