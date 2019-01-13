import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.0

import Box2D 2.0
import Chronicles 1.0

import "components"
import "gameitems"
import "gameoverlays"
import "physics"

GamePage {
    id: gamePage

    Component.onCompleted: {
        console.log("Game scene size:", gamePage.width, "/", gamePage.height, "grid size:", app.gridSize)
        forceActiveFocus()
    }

    Keys.onPressed: Game.keyPressed(event.key, event.isAutoRepeat)
    Keys.onReleased: Game.keyReleased(event.key, event.isAutoRepeat)

    // Pysical world
    World {
        id: physicsWorld
        gravity: Qt.point(0, 0)
        onStepped: Game.onTick()
        running: Game.running
    }

    Item {
        id: sceneItem
        anchors.fill: parent

        property bool gameOverlayVisible: true

        // World flickble
        Flickable {
            id: worldFlickable
            anchors.fill: parent
            contentWidth: worldItem.width
            contentHeight: worldItem.height
            enabled: false

            Item {
                id: worldItem
                width: Game.world.map.size.width * app.gridSize
                height: Game.world.map.size.height * app.gridSize

                Rectangle {
                    anchors.fill: parent
                    color: "black"
                }

                WorldBoundaries {
                    id: worldBoundaries
                    worldObject: worldItem
                }

                Repeater {
                    id: fieldRepeater
                    model: Game.world
                    delegate: FieldItem {
                        width: app.gridSize
                        height: app.gridSize
                        field: Game.world.get(model.index)
                        x: field.position.x * app.gridSize
                        y: field.position.y * app.gridSize
                    }
                }

                CharacterItem {
                    id: playerItem
                    isPlayer: true
                    character: Game.world.player

                    width: Game.world.player.size.width * app.gridSize
                    height: Game.world.player.size.height * app.gridSize

                    x: Game.world.map.playerStartPosition.x * app.gridSize
                    y: Game.world.map.playerStartPosition.y * app.gridSize
                    z: Map.Layer2Normal

                    onXChanged: {
                        Game.world.player.position = Qt.point(playerItem.x / app.gridSize, playerItem.y / app.gridSize)
                        moveCamera()
                    }

                    onYChanged: {
                        Game.world.player.position = Qt.point(playerItem.x / app.gridSize, playerItem.y / app.gridSize)
                        moveCamera()
                    }

                    function shootBulletObject(bulletObject) {
                        var bulletStartPoint = Qt.point(playerItem.x + playerItem.getBulletXOffset(),  playerItem.y + playerItem.getBulletYOffset())
                        bulletObject.damage = 10
                        bulletObject.width = app.gridSize / 2
                        bulletObject.height = bulletObject.width
                        bulletObject.rotation = playerItem.getBulletAngle()
                        bulletObject.x = bulletStartPoint.x
                        bulletObject.y = bulletStartPoint.y
                        bulletObject.z = Map.Layer2Normal
                        bulletObject.startPositionX = bulletStartPoint.x
                        bulletObject.startPositionY = bulletStartPoint.y
                        bulletObject.shootRange = 20
                        bulletObject.fireArrow = debugControls.flamesEnabled
                        bulletObject.body.linearVelocity = Qt.point(app.gridSize * Math.cos(character.angle), app.gridSize * Math.sin(character.angle))
                    }

                    function shoot() {
                        console.log(character.name + " SHOOOT!")
                        var component = Qt.createComponent("gameitems/BulletItem.qml");
                        var bulletIncubator = component.incubateObject(worldItem, { shooter: playerItem.character } )
                        if (bulletIncubator.status !== Component.Ready) {
                            bulletIncubator.onStatusChanged = function(status) {
                                if (status === Component.Ready) {
                                    shootBulletObject(bulletIncubator.object)
                                }
                            }
                        } else {
                            shootBulletObject(bulletIncubator.object)
                        }
                    }

                    // Player movement
                    Connections {
                        target: Game.world
                        onWorldPostTick: {
                            var currentVelocity = playerItem.body.linearVelocity
                            var dvx = Game.world.playerController.velocityVector().x * app.gridSize - currentVelocity.x
                            var dvy = Game.world.playerController.velocityVector().y * app.gridSize - currentVelocity.y
                            playerItem.body.applyLinearImpulse(Qt.point(playerItem.body.getMass() * dvx, playerItem.body.getMass() * dvy), playerItem.body.getWorldCenter())
                        }
                    }

                    Connections {
                        target: Game.world.playerController
                        onShoot: {
                            // TODO: check weapon, and damage
                            playerItem.shoot()
                        }
                    }

                    Component.onCompleted: moveCamera()
                }

                Repeater {
                    id: itemsRepeater
                    model: Game.world.gameItems
                    delegate: GameItem {
                        width: model.size.width * app.gridSize
                        height: model.size.height * app.gridSize
                        x: model.position.x * app.gridSize
                        y: model.position.y * app.gridSize
                        z: model.layer
                    }
                }

                Repeater {
                    id: characersRepeater
                    model: Game.world.characterItems
                    delegate: CharacterItem {
                        character: Game.world.characterItems.get(model.index)
                        width: model.size.width * app.gridSize
                        height: model.size.height * app.gridSize
                        x: model.position.x * app.gridSize
                        y: model.position.y * app.gridSize
                        z: model.layer
                    }
                }

                Repeater {
                    id: enemiesRepeater
                    model: Game.world.enemyItems
                    delegate: EnemyItem {
                        enemy: Game.world.enemyItems.get(model.index)
                        width: model.size.width * app.gridSize
                        height: model.size.height * app.gridSize
                        x: model.position.x * app.gridSize
                        y: model.position.y * app.gridSize
                        z: model.layer
                    }
                }

                FlameItem {
                    id: fireItem
                    turbulence: Game.debugging
                    enabled: true
                    width: app.gridSize * 3
                    height: app.gridSize * 3
                    x: app.gridSize * 4
                    y: app.gridSize * 4
                }
            }

            DebugDraw {
                id: debugDraw
                world: physicsWorld
                opacity: 0.4
                visible: Game.debugging
            }
        }

        MouseArea {
            id: screenMouseArea
            anchors.fill: parent
            hoverEnabled: true
            visible: Game.world.playerController.controlMode === PlayerController.ControlModeKeyBoardMouse
            enabled: Game.world.playerController.controlMode === PlayerController.ControlModeKeyBoardMouse
            onMouseXChanged: calculateAngle()
            onMouseYChanged: calculateAngle()
        }

        DebugControls {
            id: debugControls
            anchors.right: parent.right
            anchors.rightMargin: app.margins
            anchors.top: parent.top
            anchors.topMargin: app.margins
            visible: Game.debugging
            width: app.gridSize * 5
            onDruggedChanged: {
                if (drugged) {
                    druggedShader.visible = true
                    startDruggedAnimation.start()
                    shaderTimeAnimation.start()
                } else {
                    stopDruggedAnimation.start()
                }
            }
        }

        // Game overlays
        DefaultGameOverlay{
            id: defaultGameOverlay
            anchors.fill: parent
            visible: (Game.world.playerController.controlMode === PlayerController.ControlModeKeyBoardMouse ||
                      Game.world.playerController.controlMode === PlayerController.ControlModeKeyBoard) && sceneItem.gameOverlayVisible

        }

        TouchscreenGameOverlay {
            id: touchScreenGameOverlay
            anchors.fill: parent
            visible: Game.world.playerController.controlMode === PlayerController.ControlModeTouchscreen && sceneItem.gameOverlayVisible
        }

        // Game menu overlays
        InventoryOverlay {
            id: inventoryItem
            anchors.fill: parent
            opacity: 0
        }

        ConversationOverlay {
            id: conversationItem
            anchors.fill: parent
            opacity: 0
        }

        UnlockingOverlay {
            id: unlockingItem
            anchors.fill: parent
            opacity: 0
        }

        PlunderOverlay {
            id: plunderItem
            anchors.fill: parent
            opacity: 0
        }

        PauseMenuOverlay {
            id: pauseMenuItem
            anchors.fill: parent
            opacity: 0
        }

        // States
        states: [
            State {
                name: "loadingState"
                when: Game.world.state === GameWorld.StateLoading
                PropertyChanges { target: sceneItem; gameOverlayVisible: false }
                PropertyChanges { target: conversationItem; opacity: 0 }
                PropertyChanges { target: inventoryItem; opacity: 0 }
                PropertyChanges { target: unlockingItem; opacity: 0 }
                PropertyChanges { target: plunderItem; opacity: 0 }
                PropertyChanges { target: pauseMenuItem; opacity: 0 }
            },
            State {
                name: "runningState"
                when: Game.world.state === GameWorld.StateRunning
                PropertyChanges { target: sceneItem; gameOverlayVisible: true }
                PropertyChanges { target: conversationItem; opacity: 0 }
                PropertyChanges { target: inventoryItem; opacity: 0 }
                PropertyChanges { target: unlockingItem; opacity: 0 }
                PropertyChanges { target: plunderItem; opacity: 0 }
                PropertyChanges { target: pauseMenuItem; opacity: 0 }

                StateChangeScript {
                    name: "runningState"
                    script: {
                        console.log("Reset joystick")
                        touchScreenGameOverlay.reset()
                    }
                }
            },
            State {
                name: "pausedState"
                when: Game.world.state === GameWorld.StatePaused
                PropertyChanges { target: sceneItem; gameOverlayVisible: false }
                PropertyChanges { target: conversationItem; opacity: 0 }
                PropertyChanges { target: inventoryItem; opacity: 0 }
                PropertyChanges { target: unlockingItem; opacity: 0 }
                PropertyChanges { target: plunderItem; opacity: 0 }
                PropertyChanges { target: pauseMenuItem; opacity: 1 }
            },
            State {
                name: "conversationState"
                when: Game.world.state === GameWorld.StateConversation
                PropertyChanges { target: sceneItem; gameOverlayVisible: false }
                PropertyChanges { target: conversationItem; opacity: 1 }
                PropertyChanges { target: inventoryItem; opacity: 0 }
                PropertyChanges { target: unlockingItem; opacity: 0 }
                PropertyChanges { target: plunderItem; opacity: 0 }
                PropertyChanges { target: pauseMenuItem; opacity: 0 }
            },
            State {
                name: "inventoryState"
                when: Game.world.state === GameWorld.StateInventory
                PropertyChanges { target: sceneItem; gameOverlayVisible: false }
                PropertyChanges { target: conversationItem; opacity: 0 }
                PropertyChanges { target: inventoryItem; opacity: 1 }
                PropertyChanges { target: unlockingItem; opacity: 0 }
                PropertyChanges { target: plunderItem; opacity: 0 }
                PropertyChanges { target: pauseMenuItem; opacity: 0 }
            },
            State {
                name: "unlockingState"
                when: Game.world.state === GameWorld.StateUnlocking
                PropertyChanges { target: sceneItem; gameOverlayVisible: false }
                PropertyChanges { target: conversationItem; opacity: 0 }
                PropertyChanges { target: inventoryItem; opacity: 0 }
                PropertyChanges { target: unlockingItem; opacity: 1 }
                PropertyChanges { target: plunderItem; opacity: 0 }
                PropertyChanges { target: pauseMenuItem; opacity: 0 }
            },
            State {
                name: "tradeState"
                when: Game.world.state === GameWorld.StateTrade
                PropertyChanges { target: sceneItem; gameOverlayVisible: false }
                PropertyChanges { target: conversationItem; opacity: 0 }
                PropertyChanges { target: inventoryItem; opacity: 0 }
                PropertyChanges { target: unlockingItem; opacity: 0 }
                PropertyChanges { target: plunderItem; opacity: 0 }
                PropertyChanges { target: pauseMenuItem; opacity: 0 }
            },
            State {
                name: "plunderState"
                when: Game.world.state === GameWorld.StatePlunder
                PropertyChanges { target: sceneItem; gameOverlayVisible: false }
                PropertyChanges { target: conversationItem; opacity: 0 }
                PropertyChanges { target: inventoryItem; opacity: 0 }
                PropertyChanges { target: unlockingItem; opacity: 0 }
                PropertyChanges { target: plunderItem; opacity: 1 }
                PropertyChanges { target: pauseMenuItem; opacity: 0 }
            }
        ]
    }

    ShaderEffectSource {
        id: shaderEffectSource
        sourceItem: sceneItem
    }

    ShaderEffect {
        id: druggedShader
        width: parent.width
        height: parent.height
        visible: false
        property var source: shaderEffectSource
        property real amplitude: 0.02
        property real frequency: 8
        property real time: 0

        NumberAnimation on time {
            id: shaderTimeAnimation
            loops: Animation.Infinite
            from: 0
            to: Math.PI * 2
            duration: 1800
            running: false
        }

        fragmentShader: "qrc:shadereffects/wobble.frag"
    }

    PropertyAnimation {
        id: startDruggedAnimation
        target: druggedShader
        property: "amplitude"
        loops: 1
        duration: 5000
        from: 0
        to: 0.02
        onRunningChanged: if (!running) console.log("Start drugged animation finished. Fully drugged ;)")
    }

    PropertyAnimation {
        id: stopDruggedAnimation
        target: druggedShader
        property: "amplitude"
        loops: 1
        duration: 5000
        to: 0
        onRunningChanged: {
            if (!running)  {
                console.log("Stop drugged animation finished. Clean again :)")
                shaderTimeAnimation.stop()
                druggedShader.visible = false
            }
        }
    }

    function moveCamera() {
        var worldWidth = Game.world.size.width * app.gridSize
        var worldHeight = Game.world.size.height * app.gridSize

        var playerX = Game.world.player.position.x * app.gridSize
        var playerY = Game.world.player.position.y * app.gridSize

        // FIXME: do right border and bottom border
        //        // Check if we have to move the camera in x directon
        //        if (playerX >= worldFlickable.width / 2 && playerX <= worldWidth - worldFlickable.width / 2) {
        //            // In the middle, move the camera
        //            worldFlickable.contentX = playerX - worldFlickable.width / 2
        //        } else if (playerX > worldWidth - worldFlickable.width / 2) {
        //            // Right end
        //            worldFlickable.contentX = worldWidth - worldFlickable.width
        //        } else {
        //            // Left end
        //            worldFlickable.contentX = 0
        //        }

        // Check if we have to move the camera in y directon
        if (playerX >= worldFlickable.width / 2) {
            worldFlickable.contentX = playerX - worldFlickable.width / 2
        } else {
            worldFlickable.contentX = 0
        }


        // Check if we have to move the camera in y directon
        if (playerY >= worldFlickable.height / 2) {
            worldFlickable.contentY = playerY - worldFlickable.height / 2
        } else {
            worldFlickable.contentY = 0
        }
    }

    function calculateAngle() {
        if (!Game.running)
            return;

        if (Game.world.playerController.controlMode !== PlayerController.ControlModeKeyBoardMouse)
            return;

        if (!Game.world.player.movable)
            return;

        var dx = (worldFlickable.contentX + screenMouseArea.mouseX) - playerItem.x - playerItem.width / 2
        var dy = (worldFlickable.contentY + screenMouseArea.mouseY) - playerItem.y - playerItem.height / 2
        Game.world.player.angle = Math.atan2(dy , dx)
    }
}
