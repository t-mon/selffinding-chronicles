import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0

import Box2D 2.0
import Chronicles 1.0

import "../components"
import "../physics"

PhysicsItem {
    id: root

    property Enemy enemy
    property int itemType: enemy ? enemy.itemType : GameItem.TypeNone
    property bool burning: false

    antialiasing: app.antialiasing
    opacity: enemy ? (enemy.hidingPlayer ? 0.5 : 1) : 0
    onPlayerAuraRangeChanged: enemy.playerVisible = playerAuraRange
    onPlayerOnItemChanged: enemy.playerOnItem = playerOnItem
    bodyType: enemy ? (enemy.movable ? enemy.bodyType : GameObject.BodyTypeStatic) : GameObject.BodyTypeStatic
    linearDamping: 10
    fixedRotation: true

    // Enemy movement
    Connections {
        target: Game.engine
        function onEnginePostTick() {
            if (!root.enemy)
                return

            if (!root.enemy.alive)
                return

            var currentVelocity = body.linearVelocity
            var dvx = root.enemy.movementVector.x * app.velocityScale - currentVelocity.x
            var dvy = root.enemy.movementVector.y * app.velocityScale - currentVelocity.y
            body.applyLinearImpulse(Qt.point(dvx, dvy), Qt.point(root.width / 2, root.height / 2))
        }
    }

    onXChanged: {
        if (!enemy) return
        enemy.position.x = x / app.gridSize
    }

    onYChanged: {
        if (!enemy) return
        enemy.position.y = y / app.gridSize
        z = gameScene.calculateLayerValue(enemy.layer, y, height, worldItem.height)
    }

    fixtures: [
        Circle {
            id: itemBody
            radius: enemy ? enemy.physicsSize.width / 2 * app.gridSize : 0
            x: enemy ? enemy.physicsPosition.x * app.gridSize : 0
            y: enemy ? enemy.physicsPosition.y * app.gridSize : 0
            categories: enemy ? enemy.categoryFlag : GameObject.PhysicsNone
            collidesWith: enemy ? enemy.collisionFlag : GameObject.PhysicsNone
            density: 1
            friction: 1
            restitution: 0.0
        }
    ]

    function inflame() {
        if (flameFadeOutAnimation.running)
            flameFadeOutAnimation.stop()

        flameItem.opacity = 1
        flameItem.visible = true
        buringTimer.restart()
        root.burning = true
    }

    Item {
        id: frame
        anchors.fill: parent;
        rotation: -root.rotation

        Rectangle {
            id: wireFrame
            anchors.fill: parent
            color: "gray";
            border.color: "white";
            border.width: app.borderWidth / 2
            opacity: root.itemDebugEnabled ? 0.2 : 0
        }

        FlameItem {
            id: flameItem
            anchors.fill: parent
            enabled: root.burning

            Timer {
                id: buringTimer
                interval: 5000
                onTriggered: flameFadeOutAnimation.start()
            }

            PropertyAnimation {
                id: flameFadeOutAnimation
                duration: 2000
                target: flameItem
                property: "opacity"
                loops: 1
                to: 0
                onRunningChanged: {
                    if (!running) {
                        flameItem.visible = false
                        root.burning = false
                    }
                }
            }
        }

        ItemDescription {
            id: nameLabel
            anchors.bottom: frame.top
            anchors.horizontalCenter: frame.horizontalCenter
            text: enemy ? enemy.name : ""
            opacity: enemy ? (root.itemDebugEnabled ? 0.5 : (enemy.playerFocus ? 1 : 0)) : 0
        }

        Image {
            id: itemImage
            anchors.fill: frame
            mirror: enemy ? enemy.heading === Character.HeadingLeft : false
            source: enemy ? dataDirectory + enemy.imageName : ""
            opacity: root.itemDebugEnabled ? 0.5 : 1
        }

        PercentageIndicator {
            id: healthIndicator
            height: app.gridSize / 6
            width: app.gridSize * 2
            anchors.bottom: nameLabel.top
            anchors.horizontalCenter: parent.horizontalCenter
            opacity: 0
            color: app.healthColor
            percentage: enemy ? enemy.healthPercentage : 0
        }
    }

    Connections {
        id: healthIndicatorConnections
        target: root.enemy
        function onDamaged() {
            healthIndicator.opacity = 1
            healthIndicatorTimer.restart()
            damageAnimation.restart()
        }

        function onHealed() {
            healthIndicator.opacity = 1
            healthIndicatorTimer.restart()
        }

        function onKilled() {
            healthIndicator.opacity = 1
            healthIndicatorTimer.restart()
        }

        function onPlayerOnItemChanged(playerOnItem) {
            healthIndicator.opacity = 1
            healthIndicatorTimer.restart()
        }

        function onPlayerFocusChanged(playerFocus) {
            if (playerFocus) {
                healthIndicator.opacity = 1
                healthIndicatorTimer.restart()
            } if (!playerFocus) {
                healthIndicator.opacity = 0
                healthIndicatorTimer.stop()
            }
        }
    }

    Timer {
        id: burnDamageTimer
        interval: 1000
        running: root.burning
        repeat: true
        onTriggered: Game.engine.performBurnDamage(root.enemy, 2)
    }

    Timer {
        id: healthIndicatorTimer
        interval: 3000
        repeat: false
        onTriggered: healthIndicator.opacity = 0
    }


    ParallelAnimation {
        id: damageAnimation

        ScaleAnimator {
            target: damageIndicator
            duration: 400
            from: 1.2
            to: 1.5
        }

        SequentialAnimation {

            NumberAnimation {
                target: damageIndicator
                properties: "opacity"
                duration: 200
                from: 0
                to: 0.5
            }

            NumberAnimation {
                target: damageIndicator
                properties: "opacity"
                duration:  200
                to: 0
            }
        }
    }

    Rectangle {
        id: damageIndicator
        width: parent.width
        height: parent.height
        radius: parent.width / 2
        color: "transparent"
        border.color: app.healthColor
        border.width: parent.width / 6
        opacity: 0
    }

    Image {
        id: deathIndicator
        anchors.centerIn: parent
        width: parent.width / 2
        height: width
        source: dataDirectory + "/images/game/death-placeholder.png"
        visible: enemy ? !enemy.alive : false
    }

}
