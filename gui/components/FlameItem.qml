import QtQuick 2.9
import QtQuick.Particles 2.0

import Chronicles 1.0

Item {
    id: root

    property bool turbulence: false
    property bool enabled: true

    property real angle: 270
    property real angleVariation: 22
    property real magnitude: 20

    ParticleSystem {
        anchors.fill: parent
        running: root.enabled

        Turbulence {
            id: turb
            enabled: root.turbulence
            height: (parent.height / 2) - 4
            width: parent.width
            x: parent.width / 4
            anchors.fill: parent
            strength: 50
            NumberAnimation on strength{
                loops: NumberAnimation.Infinite
                duration: 1000
                from: 16
                to: 128
                easing.type: Easing.InOutBounce
            }
        }

        ImageParticle {
            groups: ["smoke"]
            source: dataDirectory + "/images/game/glowdot.png"
            color: "#11111111"
            colorVariation: 0
        }

        ImageParticle {
            groups: ["flame"]
            source: dataDirectory + "/images/game/glowdot.png"
            color: "#11ff400f"
            colorVariation: 0.2
        }

        Emitter {
            anchors.centerIn: parent
            group: "flame"
            emitRate: 120
            lifeSpan: 900
            size: 20
            endSize: 10
            sizeVariation: 10
            acceleration: PointDirection { y: -40 }
            velocity: AngleDirection {
                angle: root.angle
                magnitude: root.magnitude
                angleVariation: root.angleVariation
                magnitudeVariation: 5
            }
        }

        TrailEmitter {
            id: smoke1
            width: root.width
            height: root.height/2
            group: "smoke"
            follow: "flame"

            emitRatePerParticle: 1
            lifeSpan: 1800
            lifeSpanVariation: 400
            size: 16
            endSize: 8
            sizeVariation: 8
            acceleration: PointDirection { y: -40 }
            velocity: AngleDirection {
                angle: root.angle
                magnitude: root.magnitude * 2
                angleVariation: root.angleVariation
                magnitudeVariation: 5
            }
        }

        TrailEmitter {
            id: smoke2
            width: root.width
            height: root.height/2 - 20
            group: "smoke"
            follow: "flame"

            emitRatePerParticle: 4
            lifeSpan: 1800
            size: 36
            endSize: 24
            sizeVariation: 12
            acceleration: PointDirection { y: -40 }
            velocity: AngleDirection {
                angle: root.angle
                magnitude: root.magnitude * 2
                angleVariation: root.angleVariation
                magnitudeVariation: 5
            }
        }
    }
}
