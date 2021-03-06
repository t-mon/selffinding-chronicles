import QtQuick 2.9
import Box2D 2.0

Item {
    id: item

    property alias body: itemBody

    property point centerPosition: Qt.point(x + width / 2, y + height / 2)

    property bool playerOnItem: false
    property bool playerAuraRange: false
    property bool itemDebugEnabled: false

    // Body properties
    property alias world: itemBody.world
    property alias linearDamping: itemBody.linearDamping
    property alias angularDamping: itemBody.angularDamping
    property alias bodyType: itemBody.bodyType
    property alias bullet: itemBody.bullet
    property alias sleepingAllowed: itemBody.sleepingAllowed
    property alias fixedRotation: itemBody.fixedRotation
    property alias active: itemBody.active
    property alias awake: itemBody.awake
    property alias linearVelocity: itemBody.linearVelocity
    property alias angularVelocity: itemBody.angularVelocity
    property alias fixtures: itemBody.fixtures
    property alias gravityScale: itemBody.gravityScale

    Body {
        id: itemBody
        target: item
        world: physicsWorld
    }

}
