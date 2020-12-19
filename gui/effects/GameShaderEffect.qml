import QtQuick 2.9
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.0

ShaderEffect {
    id: root

    antialiasing: app.antialiasing

    property ShaderEffectSource worldShaderEffectSource
    property ShaderEffectSource lightsShaderEffectSource

    // Shader properties
    property var world: worldShaderEffectSource
    property var light: lightsShaderEffectSource

    // Grayscale
    property real grayscaleFactor: 0 // 1 = grayscale, 0 = full color

    // Ambient
    property real ambientBrightness: 1.0
    property color ambientLightColor: Qt.rgba(0, 0, 0, 0)

    // Stoned effect
    property bool stonedEffectEnabled: false
    property real amplitude: 0
    property real frequency: 8
    property real time: 0

    vertexShader: "qrc:shadereffects/vertexshaders/gamescene.frag"
    fragmentShader: "qrc:shadereffects/fragmentshaders/gamescene.frag"

    onStonedEffectEnabledChanged: {
        if (stonedEffectEnabled) {
            console.log("Stoned enabled")
            stonedStartAnimation.start()
            stonedShaderTimeAnimation.start()
        } else {
            console.log("Stoned disabled")
            stonedStopAnimation.start()
        }
    }

    NumberAnimation on time {
        id: stonedShaderTimeAnimation
        loops: Animation.Infinite
        from: 0
        to: Math.PI * 2
        duration: 1800
        running: false
    }

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
