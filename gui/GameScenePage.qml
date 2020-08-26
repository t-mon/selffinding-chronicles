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

    Component.onCompleted: console.log("Game scene created. Scene size:", root.width, "x", root.height, "|" , "Grid size:", app.gridSize)
    Component.onDestruction: console.log("Game scene destroy")

    property CharacterItem playerItem: null

    Connections {
        target: Game.engine
        onCurrentPlayerPositionChanged: {
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

    Item {
        id: sceneItem
        anchors.fill: parent

        property bool gameOverlayVisible: true

        // Create animation
        opacity: 0
        Behavior on opacity {
            NumberAnimation {
                duration: 1000
                easing { type: Easing.InOutExpo }
            }
        }

        Flickable {
            id: worldFlickable
            width: worldItem.width > sceneItem.width ? sceneItem.width : worldItem.width
            height: worldItem.height > sceneItem.height ? sceneItem.height : worldItem.height
            anchors.centerIn: parent
            contentWidth: worldItem.width
            contentHeight: worldItem.height
            enabled: false

            Item {
                id: worldItem
                anchors.centerIn: parent
                width: Game.engine.mapScene.map.size.width * app.gridSize
                height: Game.engine.mapScene.map.size.height * app.gridSize

                Rectangle {
                    id: backgroundRectangle
                    anchors.fill: parent
                    z: GameObject.LayerBackground - 1
                    color: Game.engine.mapScene.map.backgroundColor
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
                        color: "#66ffffff"
                    }
                }

                function calculateLayerValue(layer, itemY, itemHeight, worldHeight) {
                    if (layer === GameObject.LayerBackground) {
                        return -2
                    } else if (layer === GameObject.LayerBase) {
                        return -1
                    } else if (layer === GameObject.LayerItem) {
                        return itemY + itemHeight
                    }  else if (layer === GameObject.LayerOverlay) {
                        return worldHeight + itemHeight + 1
                    } else {
                        return itemY + itemHeight
                    }
                }

                Repeater {
                    id: gameObjectRepeater
                    model: Game.engine.mapScene.activeObjects
                    delegate: GameObject {
                        id: gameObjet
                        gameObject: Game.engine.mapScene.activeObjects.get(model.index)
                        itemDebugEnabled: debugControls.itemDebugEnabled
                        worldHeight: worldItem.height
                        width: model.size.width * app.gridSize
                        height: model.size.height * app.gridSize
                        x: model.position.x * app.gridSize
                        y: model.position.y * app.gridSize
                        z: worldItem.calculateLayerValue(model.layer, y, height, worldItem.height)
                    }
                }

                Repeater {
                    id: itemsRepeater
                    model: Game.engine.mapScene.activeItems
                    delegate: GameItem {
                        gameItem: Game.engine.mapScene.activeItems.get(model.index)
                        itemDebugEnabled: debugControls.itemDebugEnabled
                        width: model.size.width * app.gridSize
                        height: model.size.height * app.gridSize
                        x: model.position.x * app.gridSize
                        y: model.position.y * app.gridSize
                        z: worldItem.calculateLayerValue(model.layer, y, height, worldItem.height)
                    }
                }

                Repeater {
                    id: chestsRepeater
                    model: Game.engine.mapScene.activeChests
                    delegate: GameItem {
                        gameItem: Game.engine.mapScene.activeChests.get(model.index)
                        itemDebugEnabled: debugControls.itemDebugEnabled
                        width: model.size.width * app.gridSize
                        height: model.size.height * app.gridSize
                        x: model.position.x * app.gridSize
                        y: model.position.y * app.gridSize
                        z: worldItem.calculateLayerValue(model.layer, y, height, worldItem.height)
                    }
                }

                Repeater {
                    id: characersRepeater
                    model: Game.engine.mapScene.activeCharacters
                    delegate: CharacterItem {
                        id: characterItem
                        character: Game.engine.mapScene.activeCharacters.get(model.index)
                        itemDebugEnabled: debugControls.itemDebugEnabled
                        particleSystem: particles
                        width: model.size.width * app.gridSize
                        height: model.size.height * app.gridSize
                        x: model.position.x * app.gridSize
                        y: model.position.y * app.gridSize
                        z: worldItem.calculateLayerValue(model.layer, y, height, worldItem.height)
                        Component.onCompleted: {
                            if (characterItem.character.isPlayer) {
                                root.playerItem = characterItem
                            }
                        }
                    }
                }

                Repeater {
                    id: enemiesRepeater
                    model: Game.engine.mapScene.activeEnemies
                    delegate: EnemyItem {
                        itemDebugEnabled: debugControls.itemDebugEnabled
                        enemy: Game.engine.mapScene.activeEnemies.get(model.index)
                        width: model.size.width * app.gridSize
                        height: model.size.height * app.gridSize
                        x: model.position.x * app.gridSize
                        y: model.position.y * app.gridSize
                        z: worldItem.calculateLayerValue(model.layer, y, height, worldItem.height)
                    }
                }
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

        Flickable {
            id: lightsFlickable
            anchors.fill: worldFlickable
            contentWidth: worldFlickable.contentWidth
            contentHeight: worldFlickable.contentHeight
            contentX: worldFlickable.contentX
            contentY: worldFlickable.contentY
            enabled: false
            visible: false

            Item {
                id: lightItem
                anchors.centerIn: parent
                width: worldItem.width
                height: worldItem.height

                Image {
                    id: characterLight
                    source: dataDirectory + "/lights/spotlight.svg"
                    property point playerWorldPosition: getPlayerWorldPosition()
                    x: playerWorldPosition.x - width / 2
                    y: playerWorldPosition.y - height / 2
                    width: 20 * app.gridSize
                    height: width
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

        Loader {
            id: overlayLoader
            anchors.fill: parent
            active: sceneItem.gameOverlayVisible
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
            },
            State {
                name: "teleportState"
                when: Game.engine.state === Engine.StateTeleport
                PropertyChanges { target: sceneItem; gameOverlayVisible: false }
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

    ShaderEffectSource {
        id: shaderEffectSource
        width: worldItem.width > sceneItem.width ? sceneItem.width : worldItem.width
        height: worldItem.height > sceneItem.height ? sceneItem.height : worldItem.height
        anchors.centerIn: parent
        sourceItem: worldFlickable
    }

    ShaderEffect {
        id: magicShader
        visible: debugControls.magicEnabled
        anchors.fill: source
        blending: false

        property var source: shaderEffectSource
        property real blueChannel: 0.8

        fragmentShader: "qrc:shadereffects/fragmentshaders/magic.frag"
    }

    ShaderEffect {
        id: grayscaleShader
        visible: debugControls.grayscaleEnabled
        anchors.fill: source

        blending: false

        property var source: shaderEffectSource

        vertexShader: "qrc:shadereffects/vertexshaders/grayscale.frag"
        fragmentShader: "qrc:shadereffects/fragmentshaders/grayscale.frag"
    }

    //    ShaderEffect {
    //        id: lightShader
    //        visible: debugControls.lightEnabled
    //        width: parent.width
    //        height: parent.height
    //        blending: false

    //        property var source: shaderEffectSource
    //        property point playerScreenPosition: getPlayerScreenPosition()
    //        property point playerPosition: Qt.point(playerScreenPosition.x / sceneItem.width, playerScreenPosition.y / sceneItem.height)
    //        property point screenSize: Qt.point(sceneItem.width, sceneItem.height)
    //        property real screenRatio: sceneItem.width / sceneItem.height
    //        property real lightRadius: 10 * (sceneItem.width / app.gridSize) / sceneItem.width

    //        fragmentShader: "qrc:shadereffects/fragmentshaders/light.frag"
    //    }

    ShaderEffect {
        id: ambientShader
        visible: debugControls.lightEnabled
        anchors.fill: world

        blending: false

        property var world: shaderEffectSource
        property var light: ShaderEffectSource {
            sourceItem: lightsFlickable
            hideSource: true
        }

        property real ambientBrightness: debugControls.ambientBrightness
        property real alpha: 0.2

        fragmentShader: "
            varying highp vec2 qt_TexCoord0;
            uniform sampler2D world;
            uniform sampler2D light;

            uniform lowp float qt_Opacity;

            uniform lowp float ambientBrightness;
            uniform lowp float alpha;

            vec3 rgb2hsb(vec3 c);
            vec3 hsb2rgb(vec3 c);

            void main(void) {
                lowp vec4 worldTexture = texture2D(world, qt_TexCoord0);
                lowp vec4 lightTexture = texture2D(light, qt_TexCoord0);

                lowp float brightness = ambientBrightness;

                if (lightTexture.a != 0.0) {
                    brightness = mix(ambientBrightness, 1.0, lightTexture.a);
                }

                vec3 hsbWorldColor = rgb2hsb(worldTexture.rgb);
                hsbWorldColor.b = hsbWorldColor.b * brightness;
                vec3 finalColor = hsb2rgb(hsbWorldColor);
                gl_FragColor = vec4(finalColor, worldTexture.a) * qt_Opacity;
            }

            vec3 rgb2hsb(vec3 c) {
                vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
                vec4 p = mix(vec4(c.bg, K.wz),
                             vec4(c.gb, K.xy),
                             step(c.b, c.g));
                vec4 q = mix(vec4(p.xyw, c.r),
                             vec4(c.r, p.yzx),
                             step(p.x, c.r));
                float d = q.x - min(q.w, q.y);
                float e = 1.0e-10;
                return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)),
                            d / (q.x + e),
                            q.x);
            }

            vec3 hsb2rgb(vec3 c) {
                vec3 rgb = clamp(abs(mod(c.x*6.0+vec3(0.0,4.0,2.0), 6.0)-3.0)-1.0, 0.0, 1.0 );
                rgb = rgb*rgb*(3.0 - 2.0 * rgb);
                return c.z * mix(vec3(1.0), rgb, c.y);
            }
        "
    }


    ShaderEffect {
        id: stonedShader
        anchors.fill: source

        visible: false
        blending: false

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

        fragmentShader: "qrc:shadereffects/fragmentshaders/wobble.frag"
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

    // ##################################################################################
    // Debug controls on top of the scene item
    // ##################################################################################



    DebugControls {
        id: debugControls
        anchors.fill: parent
        visible: Game.debugging
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

    // ##################################################################################
    // Functions
    // ##################################################################################

    function runGame() {
        forceActiveFocus()
        evaluateViewWindow()
        moveCamera()
        particles.running = true
        sceneItem.opacity = 1
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
        var playerX = worldFlickable.x + (playerWorldPosition.x - worldFlickable.contentX)
        var playerY = worldFlickable.y + (playerWorldPosition.y - worldFlickable.contentY)
        return Qt.point(playerX, playerY)
    }

    function moveCamera() {

        // Get player position in the scene
        var playerWorldPosition = getPlayerWorldPosition()

        if (worldFlickable.width < worldItem.width) {
            // Do camera x movement
            if (playerWorldPosition.x <= worldFlickable.width / 2) {
                // Reached world left border
                worldFlickable.contentX = 0
            } else if (playerWorldPosition.x >= worldItem.width - worldFlickable.width / 2) {
                // Reached world right border
                worldFlickable.contentX = worldItem.width - worldFlickable.width
            } else {
                // Move camera
                worldFlickable.contentX = playerWorldPosition.x - worldFlickable.width / 2
            }
        }

        if (worldFlickable.height < worldItem.height) {
            // Do camera y movement
            if (playerWorldPosition.y < worldFlickable.height / 2) {
                // Reached world top border
                worldFlickable.contentY = 0
            } else if (playerWorldPosition.y > worldItem.height - worldFlickable.height / 2) {
                // Reached world bottom border
                worldFlickable.contentY = worldItem.height - worldFlickable.height
            } else {
                // Move camera
                worldFlickable.contentY = playerWorldPosition.y - worldFlickable.height / 2
            }
        }
    }

    function evaluateViewWindow() {
        var viewOffsetX = Math.round(worldFlickable.contentX / app.gridSize)
        var viewOffsetY = Math.round(worldFlickable.contentY / app.gridSize)
        var viewWindowX = Math.round(worldFlickable.width / app.gridSize)
        var viewWindowY = Math.round(worldFlickable.height / app.gridSize)

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
