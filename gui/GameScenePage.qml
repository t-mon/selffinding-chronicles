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
        console.log("Game scene created. Scene size:", root.width, "x", root.height, "|" , "Grid size:", app.gridSize)
    }
    Component.onDestruction: console.log("Game scene destroy")

    property CharacterItem playerItem: gameScene.playerItem

    Connections {
        target: Game.engine
        function onCurrentPlayerPositionChanged(currentPlayerPosition) {
            evaluateViewWindow()
        }
    }

    // Pysical world
    World {
        id: physicsWorld
        timeStep: app.gameSpeedFactor / 60 // Default 60 Hz
        onTimeStepChanged: console.log("Game speed changed to", timeStep, "[1/s]", debugControls.gameSpeed)
        gravity: Qt.point(0, 0)
        pixelsPerMeter: app.gridSize
        onStepped: Game.onTick()
        running: Game.running
    }

    // ##################################################################################
    // Game scene
    // ##################################################################################

    GameScene {
        id: gameScene
        anchors.fill: parent
        physicsWorld: physicsWorld
        mapScene: Game.engine.mapScene
        scrollBarsEnabled: false

        itemDebugEnabled: debugControls.itemDebugEnabled
        physicsDebugEnabled: debugControls.physicsDebugEnabled
        rainingEnabled: debugControls.rainingEnabled
        snowingEnabled: debugControls.snowingEnabled
        turbulenceEnabled: debugControls.turbulenceEnabled

        stonedEnabled: debugControls.stonedEnabled
        grayscaleEnabled: debugControls.grayscaleEnabled

        property bool gameOverlayVisible: true

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

        Loader {
            id: overlayLoader
            anchors.fill: parent
            active: gameScene.gameOverlayVisible
            source: {
                switch (Game.engine.playerController.controlMode) {
                case PlayerController.ControlModeTouchscreen:
                    return "gameoverlays/TouchscreenGameOverlay.qml"
                default:
                    return "gameoverlays/DefaultGameOverlay.qml"
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

        // Engine states
        states: [
            State {
                name: "loadingState"
                when: Game.engine.state === Engine.StateLoading
                PropertyChanges { target: gameScene; gameOverlayVisible: false }
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
                PropertyChanges { target: gameScene; gameOverlayVisible: true }
                PropertyChanges { target: conversationItem; opacity: 0 }
                PropertyChanges { target: inventoryItem; opacity: 0 }
                PropertyChanges { target: unlockingItem; opacity: 0 }
                PropertyChanges { target: plunderItem; opacity: 0 }
                PropertyChanges { target: readItem; opacity: 0 }
                PropertyChanges { target: pauseMenuItem; opacity: 0 }

                StateChangeScript {
                    script: {
                        if (Game.engine.playerController.controlMode == PlayerController.ControlModeTouchscreen) {
                            overlayLoader.item.reset()
                        }
                    }
                }
            },
            State {
                name: "pausedState"
                when: Game.engine.state === Engine.StatePaused
                PropertyChanges { target: gameScene; gameOverlayVisible: false }
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
                PropertyChanges { target: gameScene; gameOverlayVisible: false }
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
                PropertyChanges { target: gameScene; gameOverlayVisible: false }
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
                PropertyChanges { target: gameScene; gameOverlayVisible: false }
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
                PropertyChanges { target: gameScene; gameOverlayVisible: false }
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
                PropertyChanges { target: gameScene; gameOverlayVisible: false }
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
                PropertyChanges { target: gameScene; gameOverlayVisible: false }
                PropertyChanges { target: conversationItem; opacity: 0 }
                PropertyChanges { target: inventoryItem; opacity: 0 }
                PropertyChanges { target: unlockingItem; opacity: 0 }
                PropertyChanges { target: plunderItem; opacity: 0 }
                PropertyChanges { target: readItem; opacity: 1 }
                PropertyChanges { target: pauseMenuItem; opacity: 0 }
            },
            State {
                name: "teleportState"
                when: Game.engine.state === Engine.StateTeleport
                PropertyChanges { target: gameScene; gameOverlayVisible: false }
                PropertyChanges { target: conversationItem; opacity: 0 }
                PropertyChanges { target: inventoryItem; opacity: 0 }
                PropertyChanges { target: unlockingItem; opacity: 0 }
                PropertyChanges { target: plunderItem; opacity: 0 }
                PropertyChanges { target: readItem; opacity: 0 }
                PropertyChanges { target: pauseMenuItem; opacity: 0 }

                StateChangeScript {
                    script: {
                        console.log("Start teleport dissapear animation")
                        if (!root.playerItem) {
                            console.warn("There is currently no player character item")
                            return
                        }

                        root.playerItem.startTeleportDisappearAnimation()
                    }
                }
            }
        ]
    }

    // ##################################################################################
    // Shader effects
    // ##################################################################################

//    ShaderEffectSource {
//        id: shaderEffectSource
//        width: worldItem.width > gameScene.width ? gameScene.width : worldItem.width
//        height: worldItem.height > gameScene.height ? gameScene.height : worldItem.height
//        anchors.centerIn: parent
//        sourceItem: worldFlickable
//    }

//    ShaderEffect {
//        id: magicShader
//        visible: debugControls.magicEnabled
//        anchors.fill: source
//        blending: false

//        property var source: shaderEffectSource
//        property real blueChannel: 0.8

//        fragmentShader: "qrc:shadereffects/fragmentshaders/magic.frag"
//    }

//    ShaderEffect {
//        id: grayscaleShader
//        visible: debugControls.grayscaleEnabled
//        anchors.fill: source

//        blending: false

//        property var source: shaderEffectSource

//        vertexShader: "qrc:shadereffects/vertexshaders/grayscale.frag"
//        fragmentShader: "qrc:shadereffects/fragmentshaders/grayscale.frag"
//    }

//    //    ShaderEffect {
//    //        id: lightShader
//    //        visible: debugControls.lightEnabled
//    //        width: parent.width
//    //        height: parent.height
//    //        blending: false

//    //        property var source: shaderEffectSource
//    //        property point playerScreenPosition: getPlayerScreenPosition()
//    //        property point playerPosition: Qt.point(playerScreenPosition.x / gameScene.width, playerScreenPosition.y / gameScene.height)
//    //        property point screenSize: Qt.point(gameScene.width, gameScene.height)
//    //        property real screenRatio: gameScene.width / gameScene.height
//    //        property real lightRadius: 10 * (gameScene.width / app.gridSize) / gameScene.width

//    //        fragmentShader: "qrc:shadereffects/fragmentshaders/light.frag"
//    //    }

//    ShaderEffect {
//        id: ambientShader
//        visible: debugControls.lightEnabled
//        anchors.fill: world

//        blending: false

//        property var world: shaderEffectSource
//        property var light: ShaderEffectSource {
//            sourceItem: lightsFlickable
//            hideSource: true
//        }

//        property real ambientBrightness: debugControls.ambientBrightness
//        property real alpha: 0.2

//        fragmentShader: "
//            varying highp vec2 qt_TexCoord0;
//            uniform sampler2D world;
//            uniform sampler2D light;

//            uniform lowp float qt_Opacity;

//            uniform lowp float ambientBrightness;
//            uniform lowp float alpha;

//            vec3 rgb2hsb(vec3 c);
//            vec3 hsb2rgb(vec3 c);

//            void main(void) {
//                lowp vec4 worldTexture = texture2D(world, qt_TexCoord0);
//                lowp vec4 lightTexture = texture2D(light, qt_TexCoord0);

//                lowp float brightness = ambientBrightness;

//                if (lightTexture.a != 0.0) {
//                    brightness = mix(ambientBrightness, 1.0, lightTexture.a);
//                }

//                vec3 hsbWorldColor = rgb2hsb(worldTexture.rgb);
//                hsbWorldColor.b = hsbWorldColor.b * brightness;
//                vec3 finalColor = hsb2rgb(hsbWorldColor);
//                gl_FragColor = vec4(finalColor, worldTexture.a) * qt_Opacity;
//            }

//            vec3 rgb2hsb(vec3 c) {
//                vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
//                vec4 p = mix(vec4(c.bg, K.wz),
//                             vec4(c.gb, K.xy),
//                             step(c.b, c.g));
//                vec4 q = mix(vec4(p.xyw, c.r),
//                             vec4(c.r, p.yzx),
//                             step(p.x, c.r));
//                float d = q.x - min(q.w, q.y);
//                float e = 1.0e-10;
//                return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)),
//                            d / (q.x + e),
//                            q.x);
//            }

//            vec3 hsb2rgb(vec3 c) {
//                vec3 rgb = clamp(abs(mod(c.x*6.0+vec3(0.0,4.0,2.0), 6.0)-3.0)-1.0, 0.0, 1.0 );
//                rgb = rgb*rgb*(3.0 - 2.0 * rgb);
//                return c.z * mix(vec3(1.0), rgb, c.y);
//            }
//        "
//    }



    // ##################################################################################
    // Debug controls on top of the scene item
    // ##################################################################################

    DebugControls {
        id: debugControls
        anchors.fill: parent
        visible: Game.debugging
    }

    // ##################################################################################
    // Functions
    // ##################################################################################

    function runGame() {
        forceActiveFocus()
        evaluateViewWindow()
        moveCamera()
        gameScene.particlesRunning = true
        gameScene.opacity = 1
        Game.engine.resumeGame()
        Game.running = true
    }

    function getPlayerWorldPosition() {
        var playerX = Game.engine.player.centerPosition.x * app.gridSize
        var playerY = Game.engine.player.centerPosition.y * app.gridSize
        return Qt.point(playerX, playerY)
    }

    function getPlayerScreenPosition() {
        var playerWorldPosition = getPlayerWorldPosition()
        var playerX = gameScene.flickable.x + (playerWorldPosition.x - gameScene.flickable.contentX)
        var playerY = gameScene.flickable.y + (playerWorldPosition.y - gameScene.flickable.contentY)
        return Qt.point(playerX, playerY)
    }

    function moveCamera() {

        // Get player position in the scene
        var playerWorldPosition = getPlayerWorldPosition()
        //console.log("#############", playerWorldPosition.x, playerWorldPosition.y)
        if (gameScene.flickable.width < gameScene.world.width) {
            // Do camera x movement
            if (playerWorldPosition.x <= gameScene.flickable.width / 2) {
                // Reached world left border
                gameScene.flickable.contentX = 0
            } else if (playerWorldPosition.x >= gameScene.world.width - gameScene.flickable.width / 2) {
                // Reached world right border
                gameScene.flickable.contentX = gameScene.world.width - gameScene.flickable.width
            } else {
                // Move camera
                gameScene.flickable.contentX = playerWorldPosition.x - gameScene.flickable.width / 2
            }
        }

        if (gameScene.flickable.height < gameScene.world.height) {
            // Do camera y movement
            if (playerWorldPosition.y < gameScene.flickable.height / 2) {
                // Reached world top border
                gameScene.flickable.contentY = 0
            } else if (playerWorldPosition.y > gameScene.world.height - gameScene.flickable.height / 2) {
                // Reached world bottom border
                gameScene.flickable.contentY = gameScene.world.height - gameScene.flickable.height
            } else {
                // Move camera
                gameScene.flickable.contentY = playerWorldPosition.y - gameScene.flickable.height / 2
            }
        }
    }

    function evaluateViewWindow() {
        var viewOffsetX = Math.round(gameScene.flickable.contentX / app.gridSize)
        var viewOffsetY = Math.round(gameScene.flickable.contentY / app.gridSize)
        var viewWindowX = Math.round(gameScene.flickable.width / app.gridSize)
        var viewWindowY = Math.round(gameScene.flickable.height / app.gridSize)

        Game.engine.mapScene.viewWindow = Qt.rect(viewOffsetX - app.viewActiveFrameWidth,
                                                  viewOffsetY - app.viewActiveFrameWidth,
                                                  viewWindowX + (2 * app.viewActiveFrameWidth),
                                                  viewWindowY + (2 * app.viewActiveFrameWidth))
    }

    function calculateAngle() {
        if (!Game.running)
            return;

        if (Game.engine.playerController.controlMode !== PlayerController.ControlModeKeyBoardMouse)
            return;

        if (!Game.engine.player.movable)
            return;

        // Get player position in the scene
        var playerScenePosition = getPlayerScreenPosition()
        var dx = screenMouseArea.mouseX - playerScenePosition.x
        var dy = screenMouseArea.mouseY - playerScenePosition.y
        Game.engine.player.angle = Math.atan2(dy , dx)
    }
}
