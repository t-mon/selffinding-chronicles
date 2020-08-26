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

    property bool gameOverlayVisible: true
    property bool flickable: worldFlickable

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
        width: worldItem.width > root.width ? root.width : worldItem.width
        height: worldItem.height > root.height ? root.height : worldItem.height
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
}
