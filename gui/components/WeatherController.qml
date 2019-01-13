import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtQuick.Particles 2.12

import Chronicles 1.0

Item {
    id: root

    property bool snowing: false
    property bool raining: false

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

        ImageParticle {
            id: raindropParticle
            system: rainParticleSystem
            source: dataDirectory + "/images/game/raindrop.svg"
            colorVariation: 0
        }

        Emitter {
            anchors.fill: parent
            system: rainParticleSystem
            emitRate: 1000
            lifeSpan: 2000
            size: 20
            sizeVariation: 10
            velocity: PointDirection {
                y: 60
                yVariation: 10
            }
            acceleration: PointDirection { y: 40 }
        }
    }

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
            emitRate: 1000
            lifeSpan: 3000
            size: 8
            sizeVariation: 2
            velocity: PointDirection {
                y: 10
                yVariation: 8
            }
            acceleration: PointDirection { y: 3 }
        }
    }

}
