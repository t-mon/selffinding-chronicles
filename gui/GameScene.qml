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
import "effects"

Item {
    id: root

    property MapScene mapScene
    property World physicsWorld

    // Appearance controls
    property bool scrollBarsEnabled: false
    property bool itemDebugEnabled: false
    property bool physicsDebugEnabled: false
    property bool particlesRunning: false

    // Ambient contols

    // Weather controls
    property bool rainingEnabled: false
    property bool snowingEnabled: false
    property bool turbulenceEnabled: false

    // Shader controls
    property bool stonedEnabled: false
    property bool grayscaleEnabled: false

    // Read only
    readonly property Flickable flickable: worldFlickable
    readonly property Item world: worldItem
    readonly property Repeater items: itemsRepeater

    property CharacterItem playerItem: null

    opacity: 0
    Behavior on opacity {
        NumberAnimation {
            duration: 1000
            easing { type: Easing.InOutExpo }
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

    Flickable {
        id: worldFlickable
        width: worldItem.width > root.width ? root.width : worldItem.width
        height: worldItem.height > root.height ? root.height : worldItem.height
        anchors.centerIn: parent
        contentWidth: worldItem.width
        contentHeight: worldItem.height
        enabled: false
        visible: true

        // Only visible in the editor
        ScrollBar.vertical: ScrollBar {
            visible: scrollBarsEnabled
            enabled: scrollBarsEnabled
            active: scrollBarsEnabled
            interactive: true
            policy: ScrollBar.AlwaysOn
        }

        // Only visible in the editor
        ScrollBar.horizontal: ScrollBar {
            visible: scrollBarsEnabled
            enabled: scrollBarsEnabled
            active: scrollBarsEnabled
            interactive: true
            policy: ScrollBar.AlwaysOn
        }


        // Background items
        Rectangle {
            id: backgroundRectangle
            anchors.centerIn: parent
            width: mapScene.map.size.width * app.gridSize
            height: mapScene.map.size.height * app.gridSize
            color: mapScene.map.backgroundColor
        }

        Item {
            id: worldItem
            anchors.centerIn: parent
            width: mapScene.map.size.width * app.gridSize
            height: mapScene.map.size.height * app.gridSize

            Repeater {
                id: backgroundLightsRepeater
                model: mapScene.activeBackgroundLights
                delegate: LightSourceItem {
                    id: lightSource
                    light: Game.castLightSourceObject(mapScene.activeBackgroundLights.get(model.index))
                    itemDebugEnabled: root.itemDebugEnabled
                    width: model.size.width * app.gridSize
                    height: model.size.height * app.gridSize
                    x: model.position.x * app.gridSize
                    y: model.position.y * app.gridSize
                }
            }

            Repeater {
                id: gameObjectRepeater
                model: mapScene.activeObjects
                delegate: GameObject {
                    id: gameObjet
                    gameObject: mapScene.activeObjects.get(model.index)
                    itemDebugEnabled: root.itemDebugEnabled
                    width: model.size.width * app.gridSize
                    height: model.size.height * app.gridSize
                    x: model.position.x * app.gridSize
                    y: model.position.y * app.gridSize
                    z: gameScene.calculateLayerValue(model.layer, y, height, worldItem.height)
                }
            }
        }

        // World items
        Item {
            id: itemFlickableContent
            anchors.centerIn: parent
            width: worldItem.width
            height: worldItem.height

            WorldBoundaries {
                id: worldBoundaries
                worldObject: worldItem
            }

            ParticleSystem {
                id: particles
                anchors.fill: parent
                running: root.particlesRunning

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


            Repeater {
                id: itemsRepeater
                model: mapScene.activeItems
                delegate: GameItem {
                    gameItem: mapScene.activeItems.get(model.index)
                    itemDebugEnabled: root.itemDebugEnabled
                    width: model.size.width * app.gridSize
                    height: model.size.height * app.gridSize
                    x: model.position.x * app.gridSize
                    y: model.position.y * app.gridSize
                }
            }

            Repeater {
                id: chestsRepeater
                model: mapScene.activeChests
                delegate: GameItem {
                    gameItem: mapScene.activeChests.get(model.index)
                    itemDebugEnabled: root.itemDebugEnabled
                    width: model.size.width * app.gridSize
                    height: model.size.height * app.gridSize
                    x: model.position.x * app.gridSize
                    y: model.position.y * app.gridSize
                }
            }

            Repeater {
                id: characersRepeater
                model: mapScene.activeCharacters
                delegate: CharacterItem {
                    character: mapScene.activeCharacters.get(model.index)
                    itemDebugEnabled: root.itemDebugEnabled
                    particleSystem: particles
                    width: model.size.width * app.gridSize
                    height: model.size.height * app.gridSize
                    x: model.position.x * app.gridSize
                    y: model.position.y * app.gridSize
                    Component.onCompleted: {
                        // Get the player item for this scene
                        if (character && character.isPlayer) {
                            root.playerItem = this
                        }
                    }
                }
            }

            Repeater {
                id: enemiesRepeater
                model: mapScene.activeEnemies
                delegate: EnemyItem {
                    itemDebugEnabled: root.itemDebugEnabled
                    enemy: mapScene.activeEnemies.get(model.index)
                    width: model.size.width * app.gridSize
                    height: model.size.height * app.gridSize
                    x: model.position.x * app.gridSize
                    y: model.position.y * app.gridSize
                }
            }
        }

        Weather {
            id: weather
            anchors.fill: parent
            raining: root.rainingEnabled
            snowing: root.snowingEnabled
            turbulence: root.turbulenceEnabled
        }

        Loader {
            id: physicsDebugDrawLoader
            anchors.fill: parent
            active: root.physicsDebugEnabled
            sourceComponent: debugDrawComponent
            Component {
                id: debugDrawComponent
                DebugDraw {
                    id: debugDraw
                    world: root.physicsWorld
                    opacity: 0.4
                }
            }
        }
    }

    // ##################################################################################
    // Shader effects
    // ##################################################################################

    GrayscaleEffect {
        id: grayscaleShader
        effectEnabled: root.grayscaleEnabled
        width: worldFlickable.width
        height: worldFlickable.height
        anchors.centerIn: parent

        shaderEffectSource: ShaderEffectSource {
            width: worldFlickable.width
            height: worldFlickable.height
            hideSource: true
            sourceItem: StonedEffect {
                id: stonedEffect
                width: worldFlickable.width
                height: worldFlickable.height
                anchors.centerIn: parent
                effectEnabled: root.stonedEnabled

                shaderEffectSource: ShaderEffectSource {
                    id: mainEffectSource
                    width: worldFlickable.width
                    height: worldFlickable.height
                    hideSource: true
                    sourceItem: worldFlickable
                }
            }
        }
    }


    //    ShaderEffect {
    //        id: worldShaderEffect
    //        anchors.fill: worldFlickable
    //        width: worldFlickable.width
    //        height: worldFlickable.height
    //        anchors.centerIn: parent

    //        z: gameScene.calculateLayerValue(GameObject.LayerBackground, y, height, worldItem.height)

    //        property var source: ShaderEffectSource {
    //            id: worldShaderEffectSource
    //            sourceItem: worldFlickable
    //            hideSource: true
    //        }

    //        property var backgroundColor: mapScene.map.backgroundColor

    //        vertexShader: "qrc:shadereffects/vertexshaders/default.frag"
    //        fragmentShader: "
    //            varying highp vec2 coordinate;
    //            uniform sampler2D source;
    //            uniform lowp float qt_Opacity;
    //            uniform highp vec4 backgroundColor;

    //            void main() {
    //                highp vec4 lightFragment = texture2D(source, coordinate);
    //                // Mix the background color with the pixel from the light texture
    //                highp vec4 ambientColor = backgroundColor.rgba + lightFragment.rgba;

    //                gl_FragColor = ambientColor * qt_Opacity;
    //            }
    //        "
    //    }

    // ##################################################################################
    // Items
    // ##################################################################################

    //    // Renders the items and objects on top of the background render
    //    Flickable {
    //        id: itemFlickable
    //        anchors.fill: worldFlickable
    //        contentWidth: worldFlickable.contentWidth
    //        contentHeight: worldFlickable.contentHeight
    //        contentX: worldFlickable.contentX
    //        contentY: worldFlickable.contentY
    //        enabled: false
    //        visible: true

    //        //z: gameScene.calculateLayerValue(GameObject.LayerItems, y, height, worldItem.height)

    //        Item {
    //            id: itemFlickableContent
    //            anchors.centerIn: parent
    //            width: worldItem.width
    //            height: worldItem.height

    //            WorldBoundaries {
    //                id: worldBoundaries
    //                worldObject: worldItem
    //            }

    //            ParticleSystem {
    //                id: particles
    //                anchors.fill: parent
    //                running: root.particlesRunning

    //                ImageParticle {
    //                    id: flameImageParticle
    //                    groups: ["flame"]
    //                    source: dataDirectory + "/images/game/glowdot.png"
    //                    color: "#11ff400f"
    //                    colorVariation: 0.2
    //                }

    //                ImageParticle {
    //                    id: footstepImageParticle
    //                    groups: ["footstep"]
    //                    source: dataDirectory + "/images/characters/footstep.png"
    //                    autoRotation: true
    //                    color: "#66ffffff"
    //                }
    //            }


    //            Repeater {
    //                id: itemsRepeater
    //                model: mapScene.activeItems
    //                delegate: GameItem {
    //                    gameItem: mapScene.activeItems.get(model.index)
    //                    itemDebugEnabled: root.itemDebugEnabled
    //                    width: model.size.width * app.gridSize
    //                    height: model.size.height * app.gridSize
    //                    x: model.position.x * app.gridSize
    //                    y: model.position.y * app.gridSize
    //                }
    //            }

    //            Repeater {
    //                id: chestsRepeater
    //                model: mapScene.activeChests
    //                delegate: GameItem {
    //                    gameItem: mapScene.activeChests.get(model.index)
    //                    itemDebugEnabled: root.itemDebugEnabled
    //                    width: model.size.width * app.gridSize
    //                    height: model.size.height * app.gridSize
    //                    x: model.position.x * app.gridSize
    //                    y: model.position.y * app.gridSize
    //                }
    //            }

    //            Repeater {
    //                id: characersRepeater
    //                model: mapScene.activeCharacters
    //                delegate: CharacterItem {
    //                    character: mapScene.activeCharacters.get(model.index)
    //                    itemDebugEnabled: root.itemDebugEnabled
    //                    particleSystem: particles
    //                    width: model.size.width * app.gridSize
    //                    height: model.size.height * app.gridSize
    //                    x: model.position.x * app.gridSize
    //                    y: model.position.y * app.gridSize
    //                    Component.onCompleted: {
    //                        // Get the player item for this scene
    //                        if (character && character.isPlayer) {
    //                            root.playerItem = this
    //                        }
    //                    }
    //                }
    //            }

    //            Repeater {
    //                id: enemiesRepeater
    //                model: mapScene.activeEnemies
    //                delegate: EnemyItem {
    //                    itemDebugEnabled: root.itemDebugEnabled
    //                    enemy: mapScene.activeEnemies.get(model.index)
    //                    width: model.size.width * app.gridSize
    //                    height: model.size.height * app.gridSize
    //                    x: model.position.x * app.gridSize
    //                    y: model.position.y * app.gridSize
    //                }
    //            }
    //        }

    //        Weather {
    //            id: weather
    //            anchors.fill: parent
    //            raining: root.rainingEnabled
    //            snowing: root.snowingEnabled
    //            turbulence: root.turbulenceEnabled
    //        }

    //        Loader {
    //            id: physicsDebugDrawLoader
    //            anchors.fill: parent
    //            active: root.physicsDebugEnabled
    //            sourceComponent: debugDrawComponent
    //            Component {
    //                id: debugDrawComponent
    //                DebugDraw {
    //                    id: debugDraw
    //                    world: root.physicsWorld
    //                    opacity: 0.4
    //                }
    //            }
    //        }
    //    }


    //    Flickable {
    //        id: lightsFlickable
    //        anchors.fill: worldFlickable
    //        contentWidth: worldFlickable.contentWidth
    //        contentHeight: worldFlickable.contentHeight
    //        contentX: worldFlickable.contentX
    //        contentY: worldFlickable.contentY
    //        enabled: false
    //        visible: false

    //        Item {
    //            id: lightItem
    //            anchors.centerIn: parent
    //            width: worldItem.width
    //            height: worldItem.height

    //            Image {
    //                id: characterLight
    //                source: dataDirectory + "/lights/spotlight.svg"
    //                property point playerWorldPosition: getPlayerWorldPosition()
    //                x: playerWorldPosition.x - width / 2
    //                y: playerWorldPosition.y - height / 2
    //                width: 20 * app.gridSize
    //                height: width
    //            }
    //        }
    //    }


    // ##################################################################################
    // GameScene render layers
    // ##################################################################################


    //    ShaderEffectSource {
    //        id: itemsShaderEffectSource
    //        width: itemFlickable.width
    //        height: itemFlickable.height
    //        anchors.centerIn: parent
    //        sourceItem: itemFlickable
    //        hideSource: true
    //    }


    //    ShaderEffect {
    //        id: itemShaderEffect
    //        anchors.fill: itemsShaderEffectSource
    //        property var source: itemsShaderEffectSource
    //        vertexShader: "qrc:shadereffects/vertexshaders/default.frag"
    //        fragmentShader: "qrc:shadereffects/fragmentshaders/default.frag"
    //    }




}
