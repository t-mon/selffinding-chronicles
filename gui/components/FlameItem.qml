import QtQuick 2.9
import QtQuick.Particles 2.0

import Box2D 2.0
import Chronicles 1.0
import "../physics"

PhysicsItem {
    id: root

    property var particleSystem
    property FireItem fireItem
    property point absolutPosition

    property bool turbulence: false
    property real angle: fireItem ? fireItem.fireAngle : 270
    property real angleVariation: fireItem ? fireItem.fireAngleVariation : 22
    property real magnitude: fireItem ? fireItem.fireMagnitude : 20

    fixedRotation: false
    bodyType: Body.Dynamic
    smooth: true
    fixtures: [
        Circle {
            categories: GameObject.PhysicsFire
            collidesWith: GameObject.PhysicsCharacter |
                          GameObject.PhysicsEnemy |
                          GameObject.PhysicsFire
            radius: root.width / 4
            density: 0
            sensor: true
            x: absolutPosition.x + width / 4
            y: absolutPosition.y + height / 4
            friction: 0
            restitution: 0
            onBeginContact: console.log("contact fire")
        }
    ]

    Emitter {
        anchors.centerIn: parent
        system: root.particleSystem
        group: "flame"
        enabled: fireItem.burning
        emitRate: 150
        lifeSpan: 1000
        size: app.gridSize / 2
        endSize: app.gridSize / 2
        sizeVariation: app.gridSize / 2
        acceleration: PointDirection { y: -40 }
        velocity: AngleDirection {
            angle: root.angle
            magnitude: root.magnitude
            angleVariation: root.angleVariation
            magnitudeVariation: 8
        }
    }

    //    Item {
    //        id: flameItem
    //        anchors.fill: parent
    //        x: flamePosition.x
    //        y: flamePosition.y
    //        Rectangle {
    //            anchors.fill: parent
    //            color: "red"
    //        }

    //    }



    //    Turbulence {
    //        id: turb
    //        system: root.particleSystem
    //        enabled: root.turbulence
    //        height: (parent.height / 2) - 4
    //        width: parent.width
    //        x: parent.width / 4
    //        anchors.fill: parent
    //        strength: 50
    //        NumberAnimation on strength {
    //            loops: NumberAnimation.Infinite
    //            duration: 1000
    //            from: 16
    //            to: 128
    //            easing.type: Easing.InOutBounce
    //        }
    //    }

    //    ImageParticle {
    //        groups: ["smoke"]
    //        system: root.particleSystem
    //        source: dataDirectory + "/images/game/glowdot.png"
    //        color: "#11111111"
    //        colorVariation: 0
    //    }



    //    TrailEmitter {
    //        id: smoke1
    //        width: root.width
    //        height: root.height
    //        anchors.centerIn: parent
    //        group: "smoke"
    //        follow: "flame"
    //        enabled: root.enabled
    //        emitRatePerParticle: 1
    //        lifeSpan: 800
    //        lifeSpanVariation: 200
    //        size: app.gridSize / 3
    //        endSize: app.gridSize / 6
    //        sizeVariation: app.gridSize / 6
    //        acceleration: PointDirection { y: -40 }
    //        velocity: AngleDirection {
    //            angle: root.angle
    //            magnitude: root.magnitude * 2
    //            angleVariation: root.angleVariation
    //            magnitudeVariation: 5
    //        }
    //    }

    //    TrailEmitter {
    //        id: smoke2
    //        width: root.width
    //        height: root.height / 2
    //        anchors.centerIn: parent
    //        group: "smoke"
    //        follow: "flame"
    //        system: root.particleSystem
    //        enabled: root.enabled

    //        emitRatePerParticle: 4
    //        lifeSpan: 1200
    //        size: app.gridSize * 1.4
    //        endSize: app.gridSize
    //        sizeVariation: app.gridSize / 2
    //        acceleration: PointDirection { y: -40 }
    //        velocity: AngleDirection {
    //            angle: root.angle
    //            magnitude: root.magnitude * 2
    //            angleVariation: root.angleVariation
    //            magnitudeVariation: 5
    //        }
    //    }

}
