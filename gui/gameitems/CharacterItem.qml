import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtQuick.Particles 2.0
import QtGraphicalEffects 1.0

import Box2D 2.0
import Chronicles 1.0

import "../components"
import "../physics"

PhysicsItem {
    id: root

    property Character character
    property bool burning: false
    property bool attackRunning: false
    property int itemType: character ? character.itemType : GameItem.TypeNone
    property real auraRadius: character ? (character.auraRange + character.size.width / 2) * app.gridSize : app.gridSize
    property real hitAttackRadius: attackRunning && character ? (character.physicsSize.width * app.gridSize / 2 + app.gridSize) : hitAttackRadiusBase
    property real hitAttackRadiusBase: root.width / 3

    property var particleSystem: null

    antialiasing: app.antialiasing
    bodyType: character ? (character.movable ? character.bodyType : GameObject.BodyTypeStatic) : GameObject.BodyTypeStatic
    onPlayerAuraRangeChanged: if (character) character.playerVisible = playerAuraRange
    linearDamping: 10
    fixedRotation: true
    rotation: character ? character.angle * 180 / Math.PI : 0

    onXChanged: {
        if (!character) return
        character.position = Qt.point(x / app.gridSize, y / app.gridSize)
    }

    onYChanged: {
        if (!character) return
        character.position = Qt.point(x / app.gridSize, y / app.gridSize)
    }

    // Character movement
    Connections {
        target: Game.engine
        onEnginePostTick: {
            if (!character)
                return

            var currentVelocity = body.linearVelocity
            var dvx = character.movementVector.x * app.gridSize - currentVelocity.x
            var dvy = character.movementVector.y * app.gridSize - currentVelocity.y
            body.applyLinearImpulse(Qt.point(dvx, dvy), body.getWorldCenter())
        }
    }

    Connections {
        target: root.character
        onHeadingChanged: evaluateSpriteState()
        onMovingChanged: evaluateSpriteState()
    }

    function evaluateSpriteState() {
        if (!root.character)
            return

        if (root.character.moving) {
            if (root.character.heading === Character.HeadingLeft) {
                characterSpriteSequence.jumpTo("running-left")
            } else {
                characterSpriteSequence.jumpTo("running-right")
            }
        } else {
            if (root.character.heading === Character.HeadingLeft) {
                characterSpriteSequence.jumpTo("idle-left")
            } else {
                characterSpriteSequence.jumpTo("idle-right")
            }
        }
    }


    fixtures: [
        Circle {
            id: bodyCircle
            radius: character ? character.physicsSize.width / 2 * app.gridSize : 0
            x: character ? character.physicsPosition.x * app.gridSize : 0
            y: character ? character.physicsPosition.y * app.gridSize : 0
            categories: character ? character.categoryFlag : 0
            collidesWith: character ? character.collisionFlag : 0
            density: 1
            friction: 0.0
            restitution: 0.0

            onBeginContact: {
                if (!character.isPlayer)
                    return

                var target = other.getBody().target
                if (target.itemType) {
                    target.playerOnItem = true
                }
            }

            onEndContact: {
                if (!character.isPlayer)
                    return

                var target = other.getBody().target
                if (target.itemType) {
                    target.playerOnItem = false
                }
            }
        },
        Circle {
            id: bodySensorCircle
            radius: character ? character.physicsSize.width / 2 * app.gridSize : 0
            x: character ? character.physicsPosition.x * app.gridSize : 0
            y: character ? character.physicsPosition.y * app.gridSize : 0
            categories: GameItem.PhysicsSensor
            collidesWith: GameItem.PhysicsAll
            density: 0.0
            friction: 0.0
            restitution: 0.0
            sensor: true

            onBeginContact: {
                if (!character.isPlayer)
                    return

                var target = other.getBody().target
                if (target.itemType) {
                    target.playerOnItem = true
                }
            }

            onEndContact: {
                if (!character.isPlayer)
                    return

                var target = other.getBody().target
                if (target.itemType) {
                    target.playerOnItem = false
                }
            }
        },
        Polygon {
            id: auraSensor
            sensor: true

            density: 0.0
            friction: 0.0
            restitution: 0.0
            categories: GameItem.PhysicsSensor
            collidesWith: GameItem.PhysicsAll

            vertices: [
                Qt.point(root.width / 2, root.height / 2),
                //Qt.point(root.width / 2 + auraRadius * Math.cos(Math.PI / 4), root.height / 2 + auraRadius * Math.sin(Math.PI / 4)),
                Qt.point(root.width / 2 + auraRadius * Math.cos(Math.PI / 8), root.height / 2 + auraRadius * Math.sin(Math.PI / 8)),
                Qt.point(root.width / 2 + auraRadius * Math.cos(0), root.height / 2 + auraRadius * Math.sin(0)),
                Qt.point(root.width / 2 + auraRadius * Math.cos(-Math.PI / 8), root.height / 2 + auraRadius * Math.sin(-Math.PI / 8)),
                //Qt.point(root.width / 2 + auraRadius * Math.cos(-Math.PI / 4), root.height / 2 + auraRadius * Math.sin(-Math.PI / 4))
            ]

            onBeginContact: {
                if (!character.isPlayer)
                    return

                var target = other.getBody().target
                if (target.itemType) {
                    target.playerAuraRange = true
                }
            }

            onEndContact: {
                if (!character)
                    return

                if (!character.isPlayer)
                    return

                var target = other.getBody().target
                if (target.itemType) {
                    target.playerAuraRange = false
                }
            }
        },
        Polygon {
            id: hitAttackSensor
            sensor: true
            density: 0.0
            friction: 0.0
            restitution: 0.0
            categories: GameItem.PhysicsSensor
            collidesWith: GameItem.PhysicsAll

            property real currentRadius: weaponHitAnimation.running ? root.hitAttackRadius : root.hitAttackRadiusBase

            vertices: [
                Qt.point(root.width / 2, root.height / 2),
                Qt.point(root.width / 2 + currentRadius * Math.cos(Math.PI / 4), root.height / 2 + currentRadius * Math.sin(Math.PI / 4)),
                Qt.point(root.width / 2 + currentRadius * Math.cos(Math.PI / 8), root.height / 2 + currentRadius * Math.sin(Math.PI / 8)),
                Qt.point(root.width / 2 + currentRadius * Math.cos(0), root.height / 2 + currentRadius * Math.sin(0)),
                Qt.point(root.width / 2 + currentRadius * Math.cos(-Math.PI / 8), root.height / 2 + currentRadius * Math.sin(-Math.PI / 8)),
                Qt.point(root.width / 2 + currentRadius * Math.cos(-Math.PI / 4), root.height / 2 + currentRadius * Math.sin(-Math.PI / 4)),
            ]

            onBeginContact: {
                if (currentRadius === root.hitAttackRadiusBase)
                    return

                var target = other.getBody().target
                console.log("Hit sensor touched", target)
                if (target.itemType && target.enemy) {
                    if (character && character.weapon) {
                        console.log("Hit enemy", target.enemy.name)
                        Game.engine.performHitAttack(root.character, Game.castEnemyToCharacter(target.enemy), character.weapon.damage)
                        return
                    }
                }/* else if (target.itemType && target.character) {
                    if (character && character.weapon) {
                        console.log("Hit character", target.character.name)
                        Game.engine.performHitAttack(root.character, target.character, character.weapon.damage)
                        return
                    }
                }*/
            }
        }
    ]

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
            opacity: root.itemDebugEnabled? 0.2 : 0
        }

        ItemDescription {
            id: nameLabel
            anchors.bottom: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            text: character ? character.name : ""
            opacity: character ? (root.itemDebugEnabled ? 0.5 : (root.character.playerFocus && !character.isPlayer ? 1 : 0)) : 0
        }

        PercentageIndicator {
            id: healthIndicator
            height: app.gridSize / 6
            width: app.gridSize * 2
            anchors.bottom: nameLabel.top
            anchors.horizontalCenter: parent.horizontalCenter
            opacity: 0

            color: app.healthColor
            percentage: character ? character.healthPercentage : 0
        }

        Item {
            id: packedWeaponItem
            visible: false
            width: root.width / 2
            height: width
            anchors.verticalCenter: parent.verticalCenter
            anchors.verticalCenterOffset: 0
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.horizontalCenterOffset: character ? (character.heading === Character.HeadingLeft ? width / 5 : -width / 5) : 0
            rotation: {
                if (!character || !character.weapon)
                    return 0

                var rotationAngle = 0
                if (character.heading === Character.HeadingLeft) {
                    rotationAngle = -130
                } else {
                    rotationAngle = 130
                }

                return rotationAngle
            }


            Rectangle {
                anchors.fill: parent
                visible: root.itemDebugEnabled
                color: "transparent"
                opacity: 0.5
                border.width: 1
                border.color: "white"
            }

            Image {
                id: packedWeaponImage
                anchors.fill: parent
                mirror: character ? character.heading === Character.HeadingLeft : false
                source: {
                    if (!character)
                        return ""

                    if (!character.weapon)
                        return ""

                    return dataDirectory + character.weapon.imageName
                }
            }
        }

        Item {
            id: packedFirearmItem
            visible: false
            width: root.width / 2
            height: width
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.horizontalCenterOffset: character ? (character.heading === Character.HeadingLeft ? width / 4 : -width / 4) : 0
            rotation: {
                if (!character || !character.firearm)
                    return 0

                var rotationAngle = 0
                if (character.heading === Character.HeadingLeft) {
                    rotationAngle = 30
                } else {
                    rotationAngle = -30
                }

                if (character.firearm.firearmType === FirearmItem.FirearmTypeBow) {
                    if (character.heading === Character.HeadingLeft) {
                        rotationAngle += 75
                    } else {
                        rotationAngle -= 75
                    }
                }
                return rotationAngle
            }

            Rectangle {
                anchors.fill: parent
                visible: root.itemDebugEnabled
                color: "transparent"
                opacity: 0.5
                border.width: 1
                border.color: "white"
            }

            Image {
                id: packedFirearmImage
                anchors.fill: parent
                mirror: character ? character.heading === Character.HeadingLeft : false
                source: {
                    if (!character)
                        return ""

                    if (!character.firearm)
                        return ""

                    return dataDirectory + character.firearm.imageName
                }
            }
        }

        FlameItem {
            id: flameItem
            width: parent.width
            height: parent.height
            anchors.left: parent.left
            anchors.bottom: parent.top
            anchors.bottomMargin: -height * 2 / 3
            enabled: root.burning && root.character.active
            angle: 270
            angleVariation: 30
            magnitude: 30
            particleSystem: root.particleSystem

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

        Emitter {
            id: footstepEmitter
            width: root.width / 4
            height: width
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            system: root.particleSystem
            group: "footstep"
            enabled: root.character.moving
            emitRate: 4
            lifeSpan: 1000
            size: app.gridSize / 2

            ImageParticle {
                id: footstepImageParticle
                groups: ["footstep"]
                system: root.particleSystem
                source: dataDirectory + "/images/characters/footstep.png"
                color: "#55ffffff"
                rotation: root.character.angle * 180 / Math.PI + 90
            }
        }

        SpriteSequence {
            id: characterSpriteSequence
            anchors.fill: frame
            opacity: root.itemDebugEnabled ? 0.5 : 1
            interpolate: false

            sprites: [
                Sprite {
                    source: dataDirectory + "/images/characters/character-run/run-left.png"
                    name: "running-left"
                    frameCount: 4
                    frameWidth: 500
                    frameHeight: 500
                    frameDuration: 200
                },
                Sprite {
                    source: dataDirectory + "/images/characters/character-run/run-right.png"
                    name: "running-right"
                    frameCount: 4
                    frameWidth: 500
                    frameHeight: 500
                    frameDuration: 200
                },
                Sprite {
                    source: dataDirectory + "/images/characters/character-idle/idle-right.png"
                    name: "idle-right"
                    frameCount: 1
                    frameWidth: 500
                    frameHeight: 500
                    frameDuration: 0
                },
                Sprite {
                    source: dataDirectory + "/images/characters/character-idle/idle-left.png"
                    name: "idle-left"
                    frameCount: 1
                    frameWidth: 500
                    frameHeight: 500
                    frameDuration: 0
                }
            ]
        }

        //        Image {
        //            id: playerImage
        //            anchors.fill: frame
        //            mirror: character ? character.heading === Character.HeadingLeft : false
        //            source: dataDirectory + "/images/characters/player-male.png"
        //            opacity: root.itemDebugEnabled ? 0.5 : 1
        //        }
    }

    Timer {
        id: burnDamageTimer
        interval: 1000
        running: root.burning
        repeat: true
        onTriggered: Game.engine.performBurnDamage(root.character, 2)
    }

    Connections {
        id: attackConnections
        target: root.character

        onShoot: {
            if (!character.firearm) {
                console.log(character.name, "can not shoot. No firearm selected.")
                return
            }

            if (character.armed !== Character.ArmedFirearm)
                return

            console.log(character.name + " SHOOOT! using", character.firearm)
            var component = Qt.createComponent("BulletItem.qml");
            var bulletIncubator = component.incubateObject(worldItem, { shooter: root.character, particleSystem: root.particleSystem } )
            if (bulletIncubator && bulletIncubator.status !== Component.Ready) {
                bulletIncubator.onStatusChanged = function(status) {
                    if (status === Component.Ready) {
                        shootBulletObject(bulletIncubator.object)
                    }
                }
            } else {
                shootBulletObject(bulletIncubator.object)
            }
        }

        onHit: {
            if (weaponHitAnimation.running)
                return

            if (character.armed !== Character.ArmedWeapon)
                return

            console.log("Hit !!!!!")
            weaponHitAnimation.start()
            healthIndicator.opacity = 1
            healthIndicatorTimer.restart()
        }
    }

    // Health indicator
    Connections {
        id: healthIndicatorConnections
        target: root.character
        onDamaged: {
            healthIndicator.opacity = 1
            healthIndicatorTimer.restart()
            damageAnimation.restart()
        }

        onHealed: {
            healthIndicator.opacity = 1
            healthIndicatorTimer.restart()
        }

        onKilled: {
            healthIndicator.opacity = 0
            healthIndicatorTimer.stop()
        }

        onPlayerOnItemChanged: {
            healthIndicator.opacity = 1
            healthIndicatorTimer.restart()
        }

        onPlayerFocusChanged: {
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
        id: healthIndicatorTimer
        interval: 3000
        repeat: false
        onTriggered: healthIndicator.opacity = 0
    }

    // Damage
    ParallelAnimation {
        id: damageAnimation

        ScaleAnimator {
            target: damageIndicator
            duration: 200
            from: 1.2
            to: 1.5
        }

        SequentialAnimation {

            NumberAnimation {
                target: damageIndicator
                properties: "opacity"
                duration: 100
                from: 0
                to: 0.5
            }

            NumberAnimation {
                target: damageIndicator
                properties: "opacity"
                duration:  100
                to: 0
            }
        }
    }

    Item {
        id: directionIndicator
        visible: character ? character.isPlayer : false
        width: root.auraRadius
        height: app.gridSize
        x: parent.width / 2 + height
        y: parent.height / 2 - height / 2

        Image {
            id: directionIndicatorImage
            anchors.right: parent.right
            anchors.top: parent.top
            opacity: 0.3
            width: root.auraRadius
            height: parent.height
            source: dataDirectory + "/images/game/direction-indicator.svg"
        }
    }

    Item {
        id: weaponItem
        visible: character ? character.armed === Character.ArmedWeapon : false
        width: root.width / 2
        height: width
        x: parent.width / 2
        y: parent.height / 2 - height / 2

        rotation: 0

        transform: Rotation {
            origin.x: -weaponItem.width / 2
            origin.y: weaponItem.height / 2
            angle: weaponItem.rotation
            axis { x: 0; y: 0; z: 1 }
        }

        SequentialAnimation {
            id: weaponHitAnimation
            onRunningChanged: console.log("Hit animation", running ? "running" : "stopped")
            PropertyAnimation {
                id: weaponHitSwing1Animation
                target: weaponItem
                property: "rotation"
                to: character ? (character.heading === Character.HeadingLeft ? 45 : -45) : 0
                duration: 175
                easing.type: Easing.OutQuad
            }
            ScriptAction {script: { root.attackRunning = true } }
            PropertyAnimation {
                id: weaponHitSwing2Animation
                target: weaponItem
                property: "rotation"
                to: character ? (character.heading === Character.HeadingLeft ? -45 : 45) : 0
                duration: 150
                easing.type: Easing.InOutQuart
            }
            ScriptAction {script: { root.attackRunning = false } }
            PropertyAnimation {
                id: weaponHitSwing3Animation
                target: weaponItem
                property: "rotation"
                to: 0
                duration: 175
                easing.type: Easing.InQuad
            }
        }

        Rectangle {
            anchors.fill: parent
            visible: root.itemDebugEnabled
            color: "transparent"
            opacity: 0.5
            border.width: 1
            border.color: "white"
        }

        Image {
            id: weaponImage
            anchors.fill: parent
            rotation: 90
            mirror: character ? character.heading === Character.HeadingLeft : false
            source: {
                if (!character)
                    return ""

                if (!character.weapon)
                    return ""

                return dataDirectory + character.weapon.imageName
            }
        }

        states: [
            State {
                name: "noWeapon"
                when: !character.weapon
                PropertyChanges { target: weaponItem; visible: false }
                PropertyChanges { target: packedWeaponItem; visible: false }
                StateChangeScript {
                    script: {
                        console.log("Character no weapon")
                    }
                }
            },
            State {
                name: "holdingWeapon"
                when: character.armed === Character.ArmedWeapon
                PropertyChanges { target: weaponItem; visible: true }
                PropertyChanges { target: packedWeaponItem; visible: false }
                StateChangeScript {
                    script: {
                        console.log("Character holding weapon")
                    }
                }
            },
            State {
                name: "packedWeapon"
                when: character.weapon && character.armed !== Character.ArmedWeapon
                PropertyChanges { target: weaponItem; visible: false }
                PropertyChanges { target: packedWeaponItem; visible: true }
                StateChangeScript {
                    script: {
                        console.log("Character packed weapon")
                    }
                }
            }
        ]
    }

    Item {
        id: firearmItem
        visible: false
        width: root.width / 2
        height: width
        x: parent.width / 2
        y: parent.height / 2 - height / 2

        rotation: 0

        transform: Rotation {
            origin.x: -firearmItem.width / 2
            origin.y: firearmItem.height / 2
            angle: firearmItem.rotation
            axis { x: 0; y: 0; z: 1 }
        }

        Rectangle {
            anchors.fill: parent
            visible: root.itemDebugEnabled
            color: "transparent"
            border.width: 1
            border.color: "white"
        }

        Image {
            id: firearmImage
            anchors.fill: parent
            rotation: 90
            mirror: character ? character.heading === Character.HeadingLeft : false
            source: {
                if (!character)
                    return ""

                if (!character.firearm)
                    return ""

                return dataDirectory + character.firearm.imageName
            }
        }

        states: [
            State {
                name: "noFirearm"
                when: !character.firearm
                PropertyChanges { target: firearmItem; visible: false }
                PropertyChanges { target: packedFirearmItem; visible: false }
                StateChangeScript {
                    script: {
                        console.log("Character no firearm")
                    }
                }
            },
            State {
                name: "holdingFirearm"
                when: character.armed === Character.ArmedFirearm
                PropertyChanges { target: firearmItem; visible: true }
                PropertyChanges { target: packedFirearmItem; visible: false }
                StateChangeScript {
                    script: {
                        console.log("Character holding firearm")
                    }
                }
            },
            State {
                name: "packedFirearm"
                when: character.firearm && character.armed !== Character.ArmedFirearm
                PropertyChanges { target: firearmItem; visible: false }
                PropertyChanges { target: packedFirearmItem; visible: true }
                StateChangeScript {
                    script: {
                        console.log("Character packed firearm")
                    }
                }
            }
        ]
    }

    Rectangle {
        id: damageIndicator
        width: parent.width
        height: parent.height
        radius: parent.width / 2
        color: "transparent"
        border.color: app.healthColor
        border.width: parent.width / 8
        opacity: 0
    }

    Image {
        id: deathIndicator
        anchors.centerIn: parent
        width: parent.width / 2
        height: width
        source: dataDirectory + "/images/game/death-placeholder.png"
        visible: character ? !character.alive : false
    }

    function getBulletXOffset() {
        var centerX = root.width / 2
        return (centerX + (root.width / 2 + app.gridSize / 4 + app.gridSize / 2) * Math.cos(character.angle)) - app.gridSize / 4
    }

    function getBulletYOffset() {
        var centerY = root.height / 2
        return (centerY + (root.height / 2 + app.gridSize / 4 + app.gridSize / 2) * Math.sin(character.angle)) - app.gridSize / 4
    }

    function getBulletAngle() {
        var angle = root.character.angle * 180 / Math.PI;
        return angle
    }

    function inflame() {
        if (flameFadeOutAnimation.running)
            flameFadeOutAnimation.stop()

        flameItem.opacity = 1
        flameItem.visible = true
        buringTimer.restart()
        root.burning = true
    }

    function shootBulletObject(bulletObject) {
        var bulletStartPoint = Qt.point(root.x + getBulletXOffset(),  root.y + getBulletYOffset())
        bulletObject.damage = root.character.firearm.damage
        bulletObject.width = app.gridSize / 2
        bulletObject.height = bulletObject.width
        bulletObject.rotation = getBulletAngle()
        bulletObject.x = bulletStartPoint.x
        bulletObject.y = bulletStartPoint.y
        bulletObject.z = Map.Layer2Normal
        bulletObject.startPositionX = bulletStartPoint.x
        bulletObject.startPositionY = bulletStartPoint.y
        bulletObject.shootRange = root.character.firearm.range
        bulletObject.fireArrow = debugControls.flamesEnabled
        var velocity = app.gridSize * 1.2
        bulletObject.body.linearVelocity = Qt.point(velocity * Math.cos(character.angle), velocity * Math.sin(character.angle))
    }
}
