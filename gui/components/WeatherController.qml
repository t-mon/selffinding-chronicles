import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtQuick.Particles 2.0

import Chronicles 1.0

Item {
    id: root

    property bool snowing: false
    property bool raining: false
    property bool turbulence: false

    onRainingChanged: {
        if (raining) {
            startRainingAnimation.start()
        } else {
            stopRainingAnimation.start()
        }
    }

    onSnowingChanged: {
        if (snowing) {
            startSnowingAnimation.start()
        } else {
            stopSnowingAnimation.start()
        }
    }

    // Rain
    ParticleSystem {
        id: rainParticleSystem
        anchors.fill: parent
        opacity: 0

        running: false

        onRunningChanged: {
            if (running) {
                opacity = 1
            } else {
                opacity = 0
            }
        }

        NumberAnimation {
            id: startRainingAnimation
            duration: 3000
            target: rainParticleSystem
            property: "opacity"
            from: 0
            to: 1
            onRunningChanged: if (running) rainParticleSystem.running = true
        }

        NumberAnimation {
            id: stopRainingAnimation
            duration: 3000
            target: rainParticleSystem
            property: "opacity"
            from: 1
            to: 0
            onRunningChanged: if (!running) rainParticleSystem.running = false
        }

        Turbulence {
            id: turb
            anchors.fill: parent
            enabled: root.turbulence
            height: app.gridSize * 20
            width: app.gridSize * 20
            x: 0
            y: 0
            strength: 200

            NumberAnimation on strength{
                loops: NumberAnimation.Infinite
                duration: 3000
                from: 100
                to: 300
                easing.type: Easing.InOutBounce
            }
        }

        ImageParticle {
            id: raindropParticle
            system: rainParticleSystem
            source: dataDirectory + "/images/game/raindrop.svg"
            colorVariation: 0
        }

        Emitter {
            anchors.fill: parent
            system: rainParticleSystem
            emitRate: 500
            lifeSpan: 1000
            size: app.gridSize / 2
            sizeVariation: size / 3
            velocity: PointDirection {
                y: app.gridSize * 2
                yVariation: app.gridSize
            }
            acceleration: PointDirection { y: app.gridSize }
        }
    }

    // Snow
    ParticleSystem {
        id: snowParticleSystem
        anchors.fill: parent
        opacity: 0
        running: false

        onRunningChanged: {
            if (running) {
                opacity = 1
            } else {
                opacity = 0
            }
        }

        NumberAnimation {
            id: startSnowingAnimation
            duration: 3000
            target: snowParticleSystem
            property: "opacity"
            from: 0
            to: 1
            onRunningChanged: if (running) snowParticleSystem.running = true
        }

        NumberAnimation {
            id: stopSnowingAnimation
            duration: 3000
            target: snowParticleSystem
            property: "opacity"
            from: 1
            to: 0
            onRunningChanged: if (!running) snowParticleSystem.running = false
        }

        Wander {
            id: wanderer
            system: snowParticleSystem
            affectedParameter: Wander.Position
            anchors.fill: parent
            xVariance: parent.width
            pace: app.gridSize / 2
        }

        ImageParticle {
            id: snowflakeParticle
            system: snowParticleSystem
            source: dataDirectory + "/images/game/snowflake.svg"
            colorVariation: 0.1
            color: "#11aaaaaa"
        }

        Emitter {
            anchors.fill: parent
            system: snowParticleSystem
            emitRate: 100
            lifeSpan: 2000
            size: app.gridSize / 3
            sizeVariation: size / 3
            velocity: PointDirection {
                y: app.gridSize
                yVariation: y / 2
            }
            acceleration: PointDirection { y: app.gridSize / 10 }
        }
    }
}
