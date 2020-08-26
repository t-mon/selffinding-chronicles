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
    property ParticleSystem particleSystem

    property int itemType: character ? character.itemType : GameItem.TypeNone
    property bool burning: false
    property bool itemDebugEnabled: false
    property bool hitAttackRunning: false
    property real auraRadius: character ? (character.auraRange + character.size.width / 2) * app.gridSize : app.gridSize
    property real rotationAngle: character ? character.angle * 180 / Math.PI : 0

    bodyType: character ? (character.alive ? GameObject.BodyTypeDynamic : GameObject.BodyTypeStatic) : GameObject.BodyTypeStatic
    fixedRotation: true
    linearDamping: 10
    antialiasing: app.antialiasing
    active: character ? character.active : false

    onXChanged: {
        if (!character) return
        character.position.x = x / app.gridSize
        //console.log(character.name, "position changed", x, ",", y)
        if (character.isPlayer) moveCamera()
    }

    onYChanged: {
        if (!character) return
        character.position.y = y / app.gridSize
        //console.log(character.name, "position changed", x, ",", y)
        if (character.isPlayer) moveCamera()
    }

    onPlayerAuraRangeChanged: {
        if (!character) return
        character.playerVisible = playerAuraRange
    }

    onPlayerOnItemChanged: {
        if (!character) return
        character.playerOnItem = playerOnItem
    }

    onCharacterChanged: {
        if (!character) return
        evaluateSpriteState()
    }

    Component.onCompleted: {
        if (!character) return
        evaluateSpriteState()
    }

    // Character movement
    Connections {
        target: Game.engine
        onEnginePostTick: {
            if (!character)
                return

            if (!character.alive || !character.movable)
                return

            var currentVelocity = body.linearVelocity
            var dvx = character.movementVector.x * app.velocityScale - currentVelocity.x
            var dvy = character.movementVector.y * app.velocityScale - currentVelocity.y
            //console.log("Apply linear impulse to body", currentVelocity)
            body.applyLinearImpulse(Qt.point(dvx, dvy), Qt.point(root.width / 2, root.height / 2))
        }
    }

    // Character connections
    Connections {
        target: root.character
        onHeadingChanged: evaluateSpriteState()
        onMovingChanged: evaluateSpriteState()
        onShoot: {
            if (!character.firearm) {
                console.log("Character", character.name, "can not shoot. No firearm selected.")
                return
            }

            if (character.armed !== Character.ArmedFirearm)
                return

            console.log("Character", character.name, "shoot arrow using", character.firearm.name, character.firearm.damage)
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

            console.log("Character", character.name, "hit using weapon", character.weapon.name, character.weapon.damage)
            weaponHitAnimation.start()
            healthIndicator.opacity = 1
            healthIndicatorTimer.restart()
        }

        // Health indicator
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

        onMovableChanged: {
            if (!character) return
            evaluateSpriteState()
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

    fixtures: [
        Circle {
            id: headCircle
            radius: root.width / 4
            x: root.width / 4
            y: 0
            categories: GameItem.PhysicsBodyHitbox
            collidesWith: GameItem.PhysicsSensor | GameItem.PhysicsWeapon | GameItem.PhysicsBullet | GameItem.PhysicsMagic
            density: 1.0
            friction: 0.0
            restitution: 0.0
        },
        Box {
            id: bodyBox
            width: root.width / 2
            height: width
            x: root.width / 4
            y: root.width / 4
            categories: GameItem.PhysicsBodyHitbox
            collidesWith: GameItem.PhysicsSensor | GameItem.PhysicsWeapon | GameItem.PhysicsBullet | GameItem.PhysicsMagic
            density: 1.0
            friction: 0.0
            restitution: 0.0
            sensor: true
        },
        Circle {
            id: bodyCircle
            radius: root.width / 4
            x: root.width / 4
            y: root.height - root.width / 2
            categories: character ? character.categoryFlag : 0
            collidesWith: character ? character.collisionFlag : 0
            density: 1
            friction: 0.0
            restitution: 0.0

            onBeginContact: {
                if (!root.character || !character.isPlayer)
                    return

                var target = other.getBody().target
                if (target.itemType) {
                    target.playerOnItem = true
                }
            }

            onEndContact: {
                if (!root.character || !character.isPlayer)
                    return

                var target = other.getBody().target
                if (target.itemType) {
                    target.playerOnItem = false
                }
            }
        }
    ]

    Emitter {
        id: footstepEmitter
        width: app.gridSize / 2
        height: width
        anchors.horizontalCenter: root.horizontalCenter
        anchors.bottom: root.bottom
        anchors.bottomMargin: root.height / 5
        system: root.particleSystem
        group: "footstep"
        enabled: root.character ? Game.running && root.character.moving && root.character.alive : false
        emitRate: root.character ? 4 * app.gameSpeedFactor * root.character.currentVelocity : 0
        lifeSpan: 1500 / app.gameSpeedFactor
        size: app.gridSize / 2
        sizeVariation: size * 0.1

        // Give the footsteps the correct orientation
        onEmitParticles: {
            for (var i = 0; i < particles.length; i++) {
                var particle = particles[i];
                particle.rotation = root.character.angle + Math.PI / 2
            }
        }
    }

    Glow {
        source: characterSpriteSequence
        anchors.fill: characterSpriteSequence
        radius: 8
        samples: 20
        color: "white"
        opacity: root.character && root.character.playerFocus ? 1 : 0
        Behavior on opacity { NumberAnimation { duration: 200 } }
    }

    Loader {
        anchors.fill: parent
        active: root.itemDebugEnabled
        source: "../components/ItemDebugFrame.qml"
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
        visible: percentage != 0
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

        Loader {
            anchors.fill: parent
            active: root.itemDebugEnabled
            source: "../components/ItemDebugFrame.qml"
        }

        Image {
            id: packedWeaponImage
            anchors.fill: parent
            mirror: character ? character.heading === Character.HeadingLeft : false
            source: {
                if (!character || !character.weapon)
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

        Loader {
            anchors.fill: parent
            active: root.itemDebugEnabled
            source: "../components/ItemDebugFrame.qml"
        }

        Image {
            id: packedFirearmImage
            anchors.fill: parent
            mirror: character ? character.heading === Character.HeadingLeft : false
            source: {
                if (!character || !character.firearm)
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
        enabled: root.character ? root.burning && root.character.active : false
        angle: 270
        angleVariation: 30
        magnitude: 30
        particleSystem: root.particleSystem

        Timer {
            id: buringTimer
            interval: 5000 / app.gameSpeedFactor
            onTriggered: flameFadeOutAnimation.start()
        }

        PropertyAnimation {
            id: flameFadeOutAnimation
            duration: 2000 / app.gameSpeedFactor
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

    SpriteSequence {
        id: characterSpriteSequence
        anchors.fill: parent
        opacity: root.itemDebugEnabled ? 0.5 : 1
        interpolate: false
        running: Game.running
        sprites: [
            Sprite {
                source: dataDirectory + "/images/characters/character-idle/idle-right.png"
                name: "idle-right"
                frameCount: 1
                frameWidth: 300
                frameHeight: 300
                frameDuration: 0
            },
            Sprite {
                source: dataDirectory + "/images/characters/character-idle/idle-left.png"
                name: "idle-left"
                frameCount: 1
                frameWidth: 300
                frameHeight: 300
                frameDuration: 0
            },
            Sprite {
                source: dataDirectory + "/images/characters/character-run/run-left.png"
                name: "running-left"
                frameCount: 4
                frameWidth: 300
                frameHeight: 300
                frameDuration: 200 / app.gameSpeedFactor
            },
            Sprite {
                source: dataDirectory + "/images/characters/character-run/run-right.png"
                name: "running-right"
                frameCount: 4
                frameWidth: 300
                frameHeight: 300
                frameDuration: 200 / app.gameSpeedFactor
            }
        ]
    }

    // ##################################################################################
    // Rotation item
    // ##################################################################################

    // Focus sensor
    PhysicsItem {
        id: focusPhyicsItem
        width: root.width
        height: width
        x: root.x
        y: root.y
        linearDamping: 0
        bodyType: GameObject.BodyTypeDynamic
        antialiasing: app.antialiasing
        fixedRotation: false
        active: character ? character.active : false
        rotation: root.rotationAngle
        fixtures: [
            Box {
                id: focusSensor
                width: root.auraRadius
                height: root.width / 8
                x: root.width / 2
                y: root.height / 2 - height / 2
                categories: GameItem.PhysicsSensor
                collidesWith: GameItem.PhysicsAll
                density: 0.0
                friction: 0.0
                restitution: 0.0
                sensor: true

                onBeginContact: {
                    if (!root.character || !character.isPlayer)
                        return

                    var target = other.getBody().target
                    if (target === root || !target.itemType)
                        return

                    target.playerAuraRange = true
                }

                onEndContact: {
                    if (!root.character || !character.isPlayer)
                        return

                    var target = other.getBody().target
                    if (target === root || !target.itemType)
                        return

                    target.playerAuraRange = false
                }
            }
        ]

        Component {
            id: focusItemJointComponent

            RevoluteJoint {
                id: focusItemJoint
                enableMotor: false
                collideConnected: true
                bodyA: root.body
                bodyB: focusPhyicsItem.body
                localAnchorA: Qt.point(root.width / 2, root.height / 2)
                localAnchorB: Qt.point(focusPhyicsItem.width / 2, focusPhyicsItem.height / 2)
            }
        }

        // Note: make sure the joint will be created after the 2 bodies, and destroyed before the bodies
        Loader {
            id: focusItemJointLoader
            active: false
            sourceComponent: focusItemJointComponent
        }

        Component.onCompleted: focusItemJointLoader.active = true
        Component.onDestruction: focusItemJointLoader.active = false
    }

    // Weapon sensor
    PhysicsItem {
        id: weaponPhysicsItem
        width: root.width
        height: width
        x: root.x
        y: root.y
        linearDamping: 10
        bodyType: GameObject.BodyTypeDynamic
        antialiasing: app.antialiasing
        active: character ? character.active : false
        fixedRotation: false
        rotation: root.rotationAngle + weaponItem.rotation
        enabled: root.hitAttackRunning
        fixtures: [
            Box {
                id: weaponSensor
                width: root.hitAttackRunning ? app.gridSize * 2 : 0
                height: root.width / 6
                x: root.width / 2
                y: root.height / 2 - height / 2
                categories: GameItem.PhysicsWeapon
                collidesWith: GameItem.PhysicsAll
                density: 0.0
                friction: 0.0
                restitution: 0.0
                sensor: true

                onBeginContact: {
                    if (!root.hitAttackRunning)
                        return

                    var target = other.getBody().target
                    if (target === root || target === focusPhyicsItem)
                        return

                    if (target.itemType && target.enemy) {
                        if (character && character.weapon) {
                            Game.engine.performHitAttack(root.character, Game.castEnemyToCharacter(target.enemy), character.weapon.damage)
                            return
                        }
                    } else if (target.itemType && target.character) {
                        if (character && character.weapon) {
                            Game.engine.performHitAttack(root.character, target.character, character.weapon.damage)
                            return
                        }
                    }
                }
            }
        ]

        Component {
            id: weaponItemJointComponent

            RevoluteJoint {
                id: weaponJoint
                enableMotor: false
                collideConnected: true
                bodyA: root.body
                bodyB: weaponPhysicsItem.body
                localAnchorA: Qt.point(root.width / 2, root.height / 2)
                localAnchorB: Qt.point(weaponPhysicsItem.width / 2, weaponPhysicsItem.height / 2)
            }
        }

        // Note: make sure the joint will be created after the 2 bodies, and destroyed before the bodies
        Loader {
            id: weaponItemJointLoader
            active: false
            sourceComponent: weaponItemJointComponent
        }

        Component.onCompleted: weaponItemJointLoader.active = true
        Component.onDestruction: weaponItemJointLoader.active = false
    }


    Item {
        id: rotationItem
        width: parent.width
        height: parent.height
        anchors.centerIn: parent
        rotation: root.rotationAngle

        Loader {
            anchors.fill: parent
            active: root.itemDebugEnabled
            source: "../components/ItemDebugFrame.qml"
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
                //onRunningChanged: console.log("Hit animation", running ? "running" : "stopped")
                PropertyAnimation {
                    id: weaponHitSwing1Animation
                    target: weaponItem
                    property: "rotation"
                    to: character ? (character.heading === Character.HeadingLeft ? 45 : -45) : 0
                    duration: 175
                    easing.type: Easing.OutQuad
                }
                ScriptAction { script: { root.hitAttackRunning = true } }
                PropertyAnimation {
                    id: weaponHitSwing2Animation
                    target: weaponItem
                    property: "rotation"
                    to: character ? (character.heading === Character.HeadingLeft ? -45 : 45) : 0
                    duration: 150
                    easing.type: Easing.InOutQuart
                }
                ScriptAction { script: { root.hitAttackRunning = false } }
                PropertyAnimation {
                    id: weaponHitSwing3Animation
                    target: weaponItem
                    property: "rotation"
                    to: 0
                    duration: 175
                    easing.type: Easing.InQuad
                }
            }

            Loader {
                anchors.fill: parent
                active: root.itemDebugEnabled
                source: "../components/ItemDebugFrame.qml"
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
                    when: character && !character.weapon
                    PropertyChanges { target: weaponItem; visible: false }
                    PropertyChanges { target: packedWeaponItem; visible: false }
                    StateChangeScript {
                        script: {
                            console.log("Character", character.name, "holding no weapon")
                        }
                    }
                },
                State {
                    name: "holdingWeapon"
                    when: character && character.weapon && character.armed === Character.ArmedWeapon
                    PropertyChanges { target: weaponItem; visible: true }
                    PropertyChanges { target: packedWeaponItem; visible: false }
                    StateChangeScript {
                        script: {
                            console.log("Character", character.name, "holding weapon")
                        }
                    }
                },
                State {
                    name: "packedWeapon"
                    when: character && character.weapon && character.armed !== Character.ArmedWeapon
                    PropertyChanges { target: weaponItem; visible: false }
                    PropertyChanges { target: packedWeaponItem; visible: true }
                    StateChangeScript {
                        script: {
                            console.log("Character", character.name, "packed weapon")
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

            Loader {
                anchors.fill: parent
                active: root.itemDebugEnabled
                source: "../components/ItemDebugFrame.qml"
            }

            Image {
                id: firearmImage
                anchors.fill: parent
                rotation: 90
                mirror: character ? character.heading === Character.HeadingLeft : false
                source: {
                    if (!character || !character.firearm)
                        return ""

                    return dataDirectory + character.firearm.imageName
                }
            }

            onStateChanged: console.log("Firearm weapon state changed", state)

            states: [
                State {
                    name: "noFirearm"
                    when: character && !character.firearm
                    PropertyChanges { target: firearmItem; visible: false }
                    PropertyChanges { target: packedFirearmItem; visible: false }
                    StateChangeScript {
                        script: {
                            console.log("Character", character.name, "holding no firearm")
                        }
                    }
                },
                State {
                    name: "holdingFirearm"
                    when: character && character.firearm && character.armed === Character.ArmedFirearm
                    PropertyChanges { target: firearmItem; visible: true }
                    PropertyChanges { target: packedFirearmItem; visible: false }
                    StateChangeScript {
                        script: {
                            console.log("Character", character.name, "holding firearm")
                        }
                    }
                },
                State {
                    name: "packedFirearm"
                    when: character && character.firearm && character.armed !== Character.ArmedFirearm
                    PropertyChanges { target: firearmItem; visible: false }
                    PropertyChanges { target: packedFirearmItem; visible: true }
                    StateChangeScript {
                        script: {
                            console.log("Character", character.name, "packed firearm")
                        }
                    }
                }
            ]
        }
    }

    Timer {
        id: healthIndicatorTimer
        interval: 3000 / app.gameSpeedFactor
        repeat: false
        onTriggered: healthIndicator.opacity = 0
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

    // ##################################################################################
    // Character animations
    // ##################################################################################

    ParallelAnimation {
        id: teleportApprearAnimation

        NumberAnimation {
            target: root
            properties: "opacity"
            duration: 1000
            from: 0
            to: 1
        }

        ScaleAnimator {
            target: root
            duration: 1000
            from: 0
            to: 1
            easing.type: Easing.InOutElastic;
            easing.amplitude: 2.0;
            easing.period: 1.5
        }

        onRunningChanged: {
            if (running || !character || !character.isPlayer)
                return

            console.log("Teleport appear animation finished")
            Game.engine.teleportAppearAnimationFinished()
        }
    }

    ParallelAnimation {
        id: teleportDisapprearAnimation

        NumberAnimation {
            target: root
            properties: "opacity"
            duration: 1000
            from: 1
            to: 0
        }

        ScaleAnimator {
            target: root
            duration: 1000
            from: 1
            to: 0
            easing.type: Easing.OutInElastic;
            easing.amplitude: 2.0;
            easing.period: 1.5
        }

        onRunningChanged: {
            if (running || !character || !character.isPlayer)
                return

            console.log("Teleport disappear animation finished")
            Game.engine.teleportDisappearAnimationFinished()
        }
    }

    // ##################################################################################
    // Functions
    // ##################################################################################

    function evaluateSpriteState() {
        if (!root.character)
            return

        if (!character.alive) {
            if (root.character.heading === Character.HeadingLeft) {
                characterSpriteSequence.jumpTo("idle-left")
            } else {
                characterSpriteSequence.jumpTo("idle-right")
            }
            return;
        }

        if (!character.movable) {
            if (root.character.heading === Character.HeadingLeft) {
                characterSpriteSequence.jumpTo("idle-left")
            } else {
                characterSpriteSequence.jumpTo("idle-right")
            }
            return;
        }

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

    function shootBulletObject(bulletObject) {
        var bulletStartPoint = Qt.point(root.x + getBulletXOffset(),  root.y + getBulletYOffset())
        bulletObject.damage = root.character.firearm.damage
        bulletObject.width = app.gridSize / 2
        bulletObject.height = bulletObject.width
        bulletObject.rotation = getBulletAngle()
        bulletObject.x = bulletStartPoint.x
        bulletObject.y = bulletStartPoint.y
        bulletObject.startPositionX = bulletStartPoint.x
        bulletObject.startPositionY = bulletStartPoint.y
        bulletObject.shootRange = root.character.firearm.range
        bulletObject.fireArrow = debugControls.flamesEnabled
        var velocity = 40
        bulletObject.body.linearVelocity = Qt.point(velocity * Math.cos(character.angle), velocity * Math.sin(character.angle))
    }

    function inflame() {
        if (flameFadeOutAnimation.running)
            flameFadeOutAnimation.stop()

        flameItem.opacity = 1
        flameItem.visible = true
        buringTimer.restart()
        root.burning = true
    }

    function startTeleportApprearAnimation() {
        teleportApprearAnimation.running = true
    }

    function startTeleportDisappearAnimation() {
        teleportDisapprearAnimation.running = true
    }
}
