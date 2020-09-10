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

Item {
    id: root

    property World physicsWorld
    property MapScene mapScene

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

    // Read only
    readonly property Flickable flickable: worldFlickable
    readonly property Item world: worldItem
    readonly property Repeater items: itemsRepeater

    readonly property CharacterItem playerItem: null

    clip: true
    opacity: 0
    Behavior on opacity {
        NumberAnimation {
            duration: 1000
            easing { type: Easing.InOutExpo }
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

        Item {
            id: worldItem
            anchors.centerIn: parent
            width: mapScene.map.size.width * app.gridSize
            height: mapScene.map.size.height * app.gridSize

            Rectangle {
                id: backgroundRectangle
                anchors.fill: parent
                z: GameObject.LayerBackground - 1
                color: mapScene.map.backgroundColor
            }

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
                model: mapScene.activeObjects
                delegate: GameObject {
                    id: gameObjet
                    gameObject: mapScene.activeObjects.get(model.index)
                    itemDebugEnabled: root.itemDebugEnabled
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
                model: mapScene.activeItems
                delegate: GameItem {
                    gameItem: mapScene.activeItems.get(model.index)
                    itemDebugEnabled: root.itemDebugEnabled
                    width: model.size.width * app.gridSize
                    height: model.size.height * app.gridSize
                    x: model.position.x * app.gridSize
                    y: model.position.y * app.gridSize
                    z: worldItem.calculateLayerValue(model.layer, y, height, worldItem.height)
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
                    z: worldItem.calculateLayerValue(model.layer, y, height, worldItem.height)
                }
            }

            Repeater {
                id: characersRepeater
                model: mapScene.activeCharacters
                delegate: CharacterItem {
                    id: characterItem
                    character: mapScene.activeCharacters.get(model.index)
                    itemDebugEnabled: root.itemDebugEnabled
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
                model: mapScene.activeEnemies
                delegate: EnemyItem {
                    itemDebugEnabled: root.itemDebugEnabled
                    enemy: mapScene.activeEnemies.get(model.index)
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
                    world: physicsWorld
                    opacity: 0.4
                }
            }
        }
    }

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
//        id: shaderEffectSource
//        width: worldItem.width > sceneItem.width ? sceneItem.width : worldItem.width
//        height: worldItem.height > sceneItem.height ? sceneItem.height : worldItem.height
//        anchors.centerIn: parent
//        sourceItem: worldFlickable
//    }

}
