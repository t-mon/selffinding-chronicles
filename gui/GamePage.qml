import QtQuick 2.7
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.0

import Box2D 2.0
import Chronicles 1.0

import "components"
import "gameitems"
import "gamepages"
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

                    function shoot() {
                        console.log(character.name + " SHOOOT!")
                        var bulletObject = bulletComponent.createObject(worldItem,
                                                                        {
                                                                            shooter: playerItem.character,
                                                                            damage: 10,
                                                                            x: playerItem.getBulletX(),
                                                                            y: playerItem.getBulletY(),
                                                                            z: Map.Layer2Normal,
                                                                            rotation: playerItem.getBulletAngle(),
                                                                            startPositionX: playerItem.getBulletX(),
                                                                            startPositionY: playerItem.getBulletY(),
                                                                            shootRange: 20
                                                                        } )
                        var velocity = app.gridSize
                        bulletObject.body.linearVelocity = Qt.point(velocity * Math.cos(character.angle), velocity * Math.sin(character.angle))
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
        }

        DebugDraw {
            id: debugDraw
            world: physicsWorld
            opacity: 0.4
            visible: Game.debugging
        }

        // Game overlays
        InventoryPage {
            id: inventoryItem
            anchors.fill: parent
            opacity: 0
        }

        ConversationPage {
            id: conversationItem
            anchors.fill: parent
            opacity: 0
        }

        UnlockingPage {
            id: unlockingItem
            anchors.fill: parent
            opacity: 0
        }

        PlunderPage {
            id: plunderItem
            anchors.fill: parent
            opacity: 0
        }

        // States
        states: [
            State {
                name: "loadingState"
                when: Game.world.state === GameWorld.StateLoading
                PropertyChanges { target: conversationItem; opacity: 0 }
                PropertyChanges { target: inventoryItem; opacity: 0 }
                PropertyChanges { target: unlockingItem; opacity: 0 }
                PropertyChanges { target: plunderItem; opacity: 0 }
            },
            State {
                name: "runningState"
                when: Game.world.state === GameWorld.StateRunning
                PropertyChanges { target: conversationItem; opacity: 0 }
                PropertyChanges { target: inventoryItem; opacity: 0 }
                PropertyChanges { target: unlockingItem; opacity: 0 }
                PropertyChanges { target: plunderItem; opacity: 0 }
            },
            State {
                name: "pausedState"
                when: Game.world.state === GameWorld.StatePaused
                PropertyChanges { target: conversationItem; opacity: 0 }
                PropertyChanges { target: inventoryItem; opacity: 0 }
                PropertyChanges { target: unlockingItem; opacity: 0 }
                PropertyChanges { target: plunderItem; opacity: 0 }
            },
            State {
                name: "conversationState"
                when: Game.world.state === GameWorld.StateConversation
                PropertyChanges { target: conversationItem; opacity: 1 }
                PropertyChanges { target: inventoryItem; opacity: 0 }
                PropertyChanges { target: unlockingItem; opacity: 0 }
                PropertyChanges { target: plunderItem; opacity: 0 }
            },
            State {
                name: "inventoryState"
                when: Game.world.state === GameWorld.StateInventory
                PropertyChanges { target: conversationItem; opacity: 0 }
                PropertyChanges { target: inventoryItem; opacity: 1 }
                PropertyChanges { target: unlockingItem; opacity: 0 }
                PropertyChanges { target: plunderItem; opacity: 0 }
            },
            State {
                name: "unlockingState"
                when: Game.world.state === GameWorld.StateUnlocking
                PropertyChanges { target: conversationItem; opacity: 0 }
                PropertyChanges { target: inventoryItem; opacity: 0 }
                PropertyChanges { target: unlockingItem; opacity: 1 }
                PropertyChanges { target: plunderItem; opacity: 0 }
            },
            State {
                name: "tradeState"
                when: Game.world.state === GameWorld.StateTrade
                PropertyChanges { target: conversationItem; opacity: 0 }
                PropertyChanges { target: inventoryItem; opacity: 0 }
                PropertyChanges { target: unlockingItem; opacity: 0 }
                PropertyChanges { target: plunderItem; opacity: 0 }
            },
            State {
                name: "plunderState"
                when: Game.world.state === GameWorld.StatePlunder
                PropertyChanges { target: conversationItem; opacity: 0 }
                PropertyChanges { target: inventoryItem; opacity: 0 }
                PropertyChanges { target: unlockingItem; opacity: 0 }
                PropertyChanges { target: plunderItem; opacity: 1 }
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
        visible: debugControls.drugged
        property var source: shaderEffectSource
        property real amplitude: 0.008
        property real frequency: 15
        property real time: 0
        NumberAnimation on time {
            loops: Animation.Infinite
            from: 0
            to: Math.PI * 2
            duration: 1500
            running: debugControls.drugged
            onRunningChanged: {
                if (!running) druggedShader.time = 0
            }
        }

        fragmentShader: "qrc:shadereffects/wobble.frag"
    }


    MouseArea {
        id: screenMouseArea
        anchors.fill: parent
        hoverEnabled: true
        enabled: Game.world.playerController.controlMode === PlayerController.ControlModeKeyBoardMouse
        preventStealing: Game.world.playerController.controlMode === PlayerController.ControlModeKeyBoardMouse && Game.world.state === GameWorld.StateRunning
        onMouseXChanged: calculateAngle()
        onMouseYChanged: calculateAngle()
    }

    DebugControls {
        id: debugControls
        anchors.right: parent.right
        anchors.top: parent.top

        width: app.gridSize * 10
    }

    function moveCamera() {
        var worldWidth = Game.world.size.width * app.gridSize
        var worldHeight = Game.world.size.height * app.gridSize

        // Check if we have to move the camera in x directon
        if (Game.world.player.position.x * app.gridSize >= worldFlickable.width / 2) {
            worldFlickable.contentX = Game.world.player.position.x * app.gridSize - worldFlickable.width / 2
        } else {
            worldFlickable.contentX = 0
        }

        // Check if we have to move the camera in y directon
        if (Game.world.player.position.y * app.gridSize >= worldFlickable.height / 2) {
            worldFlickable.contentY = Game.world.player.position.y * app.gridSize - worldFlickable.height / 2
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

    Component {
        id: bulletComponent

        PhysicsItem {
            id: bulletItem
            smooth: true
            width: app.gridSize / 2
            height: width
            bullet: true
            fixedRotation: false
            bodyType: Body.Dynamic

            property Character shooter
            property int damage
            property int shootRange

            property real startPositionX
            property real startPositionY

            property real maxDistance: shootRange * app.gridSize

            onXChanged: evaluateDistance()
            onYChanged: evaluateDistance()

            function evaluateDistance() {
                if (body.linearVelocity == Qt.point(0, 0) || dissapearAnimation.running)
                    return

                var distance = Math.sqrt(Math.pow(bulletItem.x - bulletItem.startPositionX, 2) + Math.pow(bulletItem.y - bulletItem.startPositionY, 2))
                //console.log("Bullet distance traveld", distance, "/", bulletItem.maxDistance)
                if (distance >= maxDistance) {
                    console.log("Bullet reached max distance", distance, ">=", bulletItem.maxDistance)
                    bulletItem.body.linearVelocity = Qt.point(0, 0)
                    dissapearAnimation.start()
                }
            }

            fixtures: Circle {
                id: circleBody
                categories: GameObject.PhysicsBullet
                collidesWith: GameObject.PhysicsCharacter | GameObject.PhysicsEnemy | GameObject.PhysicsStaticItem | GameObject.PhysicsBullet

                radius: bulletItem.width / 2
                density: 1
                friction: 0
                restitution: 0

                onBeginContact: {
                    var target = other.getBody().target
                    var victim = null
                    if (target.gameItem) {
                        // If we have a collision with an item
                        switch (target.itemType) {
                        case GameItem.TypeChest:
                            console.log("Bullet collision with chest")
                            bulletItem.body.linearVelocity = Qt.point(0, 0)
                            dissapearAnimation.start()
                            break;
                        case GameItem.TypeTree:
                            console.log("Bullet collision with tree")
                            bulletItem.body.linearVelocity = Qt.point(0, 0)
                            dissapearAnimation.start()
                            break;
                        default:
                            break
                        }
                        return
                    } else {
                        bulletItem.body.linearVelocity = Qt.point(0, 0)
                        dissapearAnimation.start()
                    }

                    if (target.enemy)
                        victim = target.enemy
                    else if (target.character)
                        victim = target.character
                    else
                        return

                    console.log("Bullet collision " + bulletItem.shooter + " --> " + victim )
                    Game.world.performShootImpact(bulletItem.shooter, victim, damage)
                    bulletItem.destroy()
                }
            }

            FlameItem {
                id: flameItem
                anchors.fill: parent
                rotation: -90
                enabled: debugControls.flamesEnabled
            }

            Image {
                id: arrowImage
                anchors.fill: parent
                source: dataDirectory + "/images/items/firearms/arrow.png"
            }

            PropertyAnimation {
                id: dissapearAnimation
                target: bulletItem
                property: "opacity"
                to: 0
                duration: 1000
                loops: 1

                onRunningChanged: {
                    if (running) {
                        flameItem.turbulence = true
                    } else {
                        bulletItem.destroy()
                    }
                }
            }
        }
    }
}
