import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtQuick.Particles 2.0
import QtGraphicalEffects 1.0

import Box2D 2.0
import Chronicles 1.0

import "components"
import "gameitems"
import "gameoverlays"
import "physics"

GamePage {
    id: root

    Keys.onPressed: Game.keyPressed(event.key, event.isAutoRepeat)
    Keys.onReleased: Game.keyReleased(event.key, event.isAutoRepeat)

    onWidthChanged: console.log("Game page widht changed.") && evaluateViewWindow()
    onHeightChanged: console.log("Game page height changed.") &&  evaluateViewWindow()

    Component.onCompleted: {
        console.log("Game scene size:", root.width, "x", root.height, "Grid size:", app.gridSize)
    }

    function runGame() {
        forceActiveFocus()
        moveCamera()
        evaluateViewWindow()
        particles.running = true
        Game.running = true
    }

    // Pysical world
    World {
        id: physicsWorld
        timeStep: 1/60 * app.gameSpeedFactor
        onTimeStepChanged: console.log("Game speed changed to", timeStep, "[1/s]", debugControls.gameSpeed)
        gravity: Qt.point(0, 0)
        onStepped: Game.onTick()
        running: Game.engine.state == Engine.StateRunning
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
                width: Game.engine.dataManager.worldSize.width * app.gridSize
                height: Game.engine.dataManager.worldSize.height * app.gridSize

                Rectangle {
                    anchors.fill: parent
                    color: Game.engine.dataManager.worldBackgroundColor
                }

                WorldBoundaries {
                    id: worldBoundaries
                    worldObject: worldItem
                }

                ParticleSystem {
                    id: particles
                    anchors.fill: parent
                    running: false

                    ImageParticle {
                        id: flameImageParticle
                        groups: ["flame"]
                        source: dataDirectory + "/images/game/glowdot.png"
                        color: "#11ff400f"
                        colorVariation: 0.2
                    }

                    ImageParticle {
                        id: footstepImageParticle
                        groups: ["footstep"]
                        source: dataDirectory + "/images/characters/footstep.png"
                        autoRotation: true
                        //source: dataDirectory + "/images/game/footstep.png"
                        color: "#66ffffff"
                    }
                }

                Repeater {
                    id: characersRepeater
                    model: Game.engine.activeCharacters
                    delegate: CharacterItem {
                        id: characterItem
                        character: Game.engine.activeCharacters.get(model.index)
                        itemDebugEnabled: debugControls.itemDebugEnabled
                        particleSystem: particles
                        width: model.size.width * app.gridSize
                        height: model.size.height * app.gridSize
                        x: model.position.x * app.gridSize
                        y: model.position.y * app.gridSize
                        z: y + height
                        onXChanged: if (character && character.isPlayer) moveCamera()
                        onYChanged: if (character && character.isPlayer) moveCamera()
                        Component.onCompleted: if (character && character.isPlayer) moveCamera()
                    }
                }

                Repeater {
                    id: itemsRepeater
                    model: Game.engine.activeItems
                    delegate: GameItem {
                        gameItem: Game.engine.activeItems.get(model.index)
                        itemDebugEnabled: debugControls.itemDebugEnabled
                        width: model.size.width * app.gridSize
                        height: model.size.height * app.gridSize
                        x: model.position.x * app.gridSize
                        y: model.position.y * app.gridSize
                        z: y + height
                    }
                }

                Repeater {
                    id: enemiesRepeater
                    model: Game.engine.activeEnemies
                    delegate: EnemyItem {
                        itemDebugEnabled: debugControls.itemDebugEnabled
                        enemy: Game.engine.activeEnemies.get(model.index)
                        width: model.size.width * app.gridSize
                        height: model.size.height * app.gridSize
                        x: model.position.x * app.gridSize
                        y: model.position.y * app.gridSize
                        z: y + height
                    }
                }

                //                Rectangle {
                //                    id: viewWindowRectangle
                //                    x: Game.engine.viewWindow.x * app.gridSize
                //                    y: Game.engine.viewWindow.y * app.gridSize
                //                    width: Game.engine.viewWindow.width * app.gridSize
                //                    height: Game.engine.viewWindow.height * app.gridSize
                //                    opacity: 0.2
                //                    color: "white"
                //                }

            }

            Weather {
                id: weather
                anchors.fill: parent
                raining: debugControls.rainingEnabled
                snowing: debugControls.snowingEnabled
                turbulence: debugControls.turbulenceEnabled
            }

            Loader {
                id: physicsDebugDrawLoader
                anchors.fill: parent
                active: debugControls.physicsDebugEnabled
                sourceComponent: debugDrawComponent

                Component {
                    id: debugDrawComponent

                    DebugDraw {
                        id: debugDraw
                        world: physicsWorld
                        opacity: 0.4
                    }
                }
            }
        }

        // ##################################################################################
        // GameScene item overlays
        // ##################################################################################

        MouseArea {
            id: screenMouseArea
            anchors.fill: parent
            hoverEnabled: true
            visible: Game.engine.playerController.controlMode === PlayerController.ControlModeKeyBoardMouse
            enabled: Game.engine.playerController.controlMode === PlayerController.ControlModeKeyBoardMouse
            onMouseXChanged: calculateAngle()
            onMouseYChanged: calculateAngle()
        }

        // Game overlays
        DefaultGameOverlay{
            id: defaultGameOverlay
            anchors.fill: parent
            visible: (Game.engine.playerController.controlMode === PlayerController.ControlModeKeyBoardMouse ||
                      Game.engine.playerController.controlMode === PlayerController.ControlModeKeyBoard) && sceneItem.gameOverlayVisible

        }

        TouchscreenGameOverlay {
            id: touchScreenGameOverlay
            anchors.fill: parent
            visible: Game.engine.playerController.controlMode === PlayerController.ControlModeTouchscreen && sceneItem.gameOverlayVisible
        }

        DebugControls {
            id: debugControls
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: app.margins
            visible: Game.debugging
            width: app.gridSize * 8
            onStonedEnabledChanged: {
                if (stonedEnabled) {
                    stonedShader.visible = true
                    stonedStartAnimation.start()
                    stonedShaderTimeAnimation.start()
                } else {
                    stonedStopAnimation.start()
                }
            }
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

        ReadLiteratureOverlay {
            id: readItem
            anchors.fill: parent
            opacity: 0
        }

        PauseMenuOverlay {
            id: pauseMenuItem
            anchors.fill: parent
            opacity: 0
        }

        // TODO: Trade overlay, magic overlay

        // States
        states: [
            State {
                name: "loadingState"
                when: Game.engine.state === Engine.StateLoading
                PropertyChanges { target: sceneItem; gameOverlayVisible: false }
                PropertyChanges { target: conversationItem; opacity: 0 }
                PropertyChanges { target: inventoryItem; opacity: 0 }
                PropertyChanges { target: unlockingItem; opacity: 0 }
                PropertyChanges { target: plunderItem; opacity: 0 }
                PropertyChanges { target: readItem; opacity: 0 }
                PropertyChanges { target: pauseMenuItem; opacity: 0 }
            },
            State {
                name: "runningState"
                when: Game.engine.state === Engine.StateRunning
                PropertyChanges { target: sceneItem; gameOverlayVisible: true }
                PropertyChanges { target: conversationItem; opacity: 0 }
                PropertyChanges { target: inventoryItem; opacity: 0 }
                PropertyChanges { target: unlockingItem; opacity: 0 }
                PropertyChanges { target: plunderItem; opacity: 0 }
                PropertyChanges { target: readItem; opacity: 0 }
                PropertyChanges { target: pauseMenuItem; opacity: 0 }

                StateChangeScript { script: { touchScreenGameOverlay.reset() } }
            },
            State {
                name: "pausedState"
                when: Game.engine.state === Engine.StatePaused
                PropertyChanges { target: sceneItem; gameOverlayVisible: false }
                PropertyChanges { target: conversationItem; opacity: 0 }
                PropertyChanges { target: inventoryItem; opacity: 0 }
                PropertyChanges { target: unlockingItem; opacity: 0 }
                PropertyChanges { target: plunderItem; opacity: 0 }
                PropertyChanges { target: readItem; opacity: 0 }
                PropertyChanges { target: pauseMenuItem; opacity: 1 }
            },
            State {
                name: "conversationState"
                when: Game.engine.state === Engine.StateConversation
                PropertyChanges { target: sceneItem; gameOverlayVisible: false }
                PropertyChanges { target: conversationItem; opacity: 1 }
                PropertyChanges { target: inventoryItem; opacity: 0 }
                PropertyChanges { target: unlockingItem; opacity: 0 }
                PropertyChanges { target: plunderItem; opacity: 0 }
                PropertyChanges { target: readItem; opacity: 0 }
                PropertyChanges { target: pauseMenuItem; opacity: 0 }
            },
            State {
                name: "inventoryState"
                when: Game.engine.state === Engine.StateInventory
                PropertyChanges { target: sceneItem; gameOverlayVisible: false }
                PropertyChanges { target: conversationItem; opacity: 0 }
                PropertyChanges { target: inventoryItem; opacity: 1 }
                PropertyChanges { target: unlockingItem; opacity: 0 }
                PropertyChanges { target: plunderItem; opacity: 0 }
                PropertyChanges { target: readItem; opacity: 0 }
                PropertyChanges { target: pauseMenuItem; opacity: 0 }
            },
            State {
                name: "unlockingState"
                when: Game.engine.state === Engine.StateUnlocking
                PropertyChanges { target: sceneItem; gameOverlayVisible: false }
                PropertyChanges { target: conversationItem; opacity: 0 }
                PropertyChanges { target: inventoryItem; opacity: 0 }
                PropertyChanges { target: unlockingItem; opacity: 1 }
                PropertyChanges { target: plunderItem; opacity: 0 }
                PropertyChanges { target: readItem; opacity: 0 }
                PropertyChanges { target: pauseMenuItem; opacity: 0 }
            },
            State {
                name: "tradeState"
                when: Game.engine.state === Engine.StateTrade
                PropertyChanges { target: sceneItem; gameOverlayVisible: false }
                PropertyChanges { target: conversationItem; opacity: 0 }
                PropertyChanges { target: inventoryItem; opacity: 0 }
                PropertyChanges { target: unlockingItem; opacity: 0 }
                PropertyChanges { target: plunderItem; opacity: 0 }
                PropertyChanges { target: readItem; opacity: 0 }
                PropertyChanges { target: pauseMenuItem; opacity: 0 }
            },
            State {
                name: "plunderState"
                when: Game.engine.state === Engine.StatePlunder
                PropertyChanges { target: sceneItem; gameOverlayVisible: false }
                PropertyChanges { target: conversationItem; opacity: 0 }
                PropertyChanges { target: inventoryItem; opacity: 0 }
                PropertyChanges { target: unlockingItem; opacity: 0 }
                PropertyChanges { target: plunderItem; opacity: 1 }
                PropertyChanges { target: readItem; opacity: 0 }
                PropertyChanges { target: pauseMenuItem; opacity: 0 }
            },
            State {
                name: "readState"
                when: Game.engine.state === Engine.StateRead
                PropertyChanges { target: sceneItem; gameOverlayVisible: false }
                PropertyChanges { target: conversationItem; opacity: 0 }
                PropertyChanges { target: inventoryItem; opacity: 0 }
                PropertyChanges { target: unlockingItem; opacity: 0 }
                PropertyChanges { target: plunderItem; opacity: 0 }
                PropertyChanges { target: readItem; opacity: 1 }
                PropertyChanges { target: pauseMenuItem; opacity: 0 }
            }
        ]
    }

    // ##################################################################################
    // GameScene
    // ##################################################################################

    ShaderEffectSource {
        id: shaderEffectSource
        sourceItem: sceneItem
    }

    ShaderEffect {
        id: magicShader
        visible: debugControls.magicEnabled
        width: parent.width
        height: parent.height
        property var source: shaderEffectSource
        property real blueChannel: 0.8

        fragmentShader: "
            varying highp vec2 qt_TexCoord0;
            uniform sampler2D source;
            uniform lowp float qt_Opacity;
            uniform lowp float blueChannel;
            void main() {
                gl_FragColor = texture2D(source, qt_TexCoord0) * vec4(0.5, 0.5, blueChannel, 1.0) * qt_Opacity;
            }"
    }

    ShaderEffect {
        id: grayscaleShader
        visible: debugControls.grayscaleEnabled
        width: parent.width
        height: parent.height
        property var source: shaderEffectSource

        vertexShader: "
              uniform highp mat4 qt_Matrix;
              attribute highp vec4 qt_Vertex;
              attribute highp vec2 qt_MultiTexCoord0;
              varying highp vec2 coord;
              void main() {
                  coord = qt_MultiTexCoord0;
                  gl_Position = qt_Matrix * qt_Vertex;
              }"
        fragmentShader: "
              varying highp vec2 coord;
              uniform sampler2D source;
              uniform lowp float qt_Opacity;
              void main() {
                  lowp vec4 tex = texture2D(source, coord);
                  gl_FragColor = vec4(vec3(dot(tex.rgb,
                                      vec3(0.344, 0.5, 0.156))),
                                           tex.a) * qt_Opacity;
              }"
    }

    ShaderEffect {
        id: stonedShader
        width: parent.width
        height: parent.height
        visible: false

        property var source: shaderEffectSource
        property real amplitude: 0.02
        property real frequency: 8
        property real time: 0

        NumberAnimation on time {
            id: stonedShaderTimeAnimation
            loops: Animation.Infinite
            from: 0
            to: Math.PI * 2
            duration: 1800
            running: false
        }

        fragmentShader: "qrc:shadereffects/wobble.frag"
    }

    PropertyAnimation {
        id: stonedStartAnimation
        target: stonedShader
        property: "amplitude"
        loops: 1
        duration: 5000 * app.gameSpeedFactor
        from: 0
        to: 0.02
        onRunningChanged: if (!running) console.log("Start stoned animation finished. Fully stoned ;)")
    }

    PropertyAnimation {
        id: stonedStopAnimation
        target: stonedShader
        property: "amplitude"
        loops: 1
        duration: 5000 * app.gameSpeedFactor
        to: 0
        onRunningChanged: {
            if (!running)  {
                console.log("Stop stoned animation finished. Clean again :)")
                stonedShaderTimeAnimation.stop()
                stonedShader.visible = false
            }
        }
    }

    function moveCamera() {
        if (!Game.engine.player)
            return

        var worldWidth = Game.engine.dataManager.worldSize.width * app.gridSize
        var worldHeight = Game.engine.dataManager.worldSize.height * app.gridSize

        var playerX = Game.engine.player.position.x * app.gridSize
        var playerY = Game.engine.player.position.y * app.gridSize

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

    Connections {
        target: Game.engine
        onCurrentPlayerPositionChanged: {
            evaluateViewWindow()
        }
    }

    function evaluateViewWindow() {
        var viewWindowX = Math.round(root.width / app.gridSize)
        var viewWindowY = Math.round(root.height / app.gridSize)
        var viewOffsetX = Math.round(worldFlickable.contentX / app.gridSize)
        var viewOffsetY = Math.round(worldFlickable.contentY / app.gridSize)

        Game.engine.viewWindow = Qt.rect(viewOffsetX - 5, viewOffsetY - 5, viewWindowX + 10, viewWindowY + 10)
    }

    function calculateAngle() {
        if (!Game.running)
            return;

        if (Game.engine.playerController.controlMode !== PlayerController.ControlModeKeyBoardMouse)
            return;

        if (!Game.engine.player.movable)
            return;

        var dx = (worldFlickable.contentX + screenMouseArea.mouseX) - (Game.engine.player.position.x * app.gridSize + Game.engine.player.size.width * app.gridSize / 2)
        var dy = (worldFlickable.contentY + screenMouseArea.mouseY) - (Game.engine.player.position.y * app.gridSize + Game.engine.player.size.height * app.gridSize / 2)
        Game.engine.player.angle = Math.atan2(dy , dx)
    }
}
