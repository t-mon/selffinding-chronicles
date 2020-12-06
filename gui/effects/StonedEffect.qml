import QtQuick 2.9
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.0

ShaderEffect {
    id: root

    property ShaderEffectSource shaderEffectSource
    property bool effectEnabled: false

    property var source: shaderEffectSource
    property real amplitude: 0
    property real frequency: 8
    property real time: 0

    onEffectEnabledChanged: {
        if (effectEnabled) {
            console.log("Stoned enabled")
            stonedStartAnimation.start()
            stonedShaderTimeAnimation.start()
        } else {
            console.log("Stoned disabled")
            stonedStopAnimation.start()
        }
    }

    blending: false

    NumberAnimation on time {
        id: stonedShaderTimeAnimation
        loops: Animation.Infinite
        from: 0
        to: Math.PI * 2
        duration: 1800
        running: false
    }

    fragmentShader: "qrc:shadereffects/fragmentshaders/wobble.frag"


    PropertyAnimation {
        id: stonedStartAnimation
        target: root
        property: "amplitude"
        loops: 1
        duration: 5000 * app.gameSpeedFactor
        from: 0
        to: 0.02
        onRunningChanged: if (!running) console.log("Start stoned animation finished. Fully stoned ;)")
    }

    PropertyAnimation {
        id: stonedStopAnimation
        target: root
        property: "amplitude"
        loops: 1
        duration: 5000 * app.gameSpeedFactor
        to: 0
        onRunningChanged: {
            if (!running)  {
                console.log("Stop stoned animation finished. Clean again :)")
                stonedShaderTimeAnimation.stop()
                root.amplitude = 0
            }
        }
    }

}
