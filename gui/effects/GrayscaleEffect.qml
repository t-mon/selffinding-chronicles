import QtQuick 2.9
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.0

ShaderEffect {
    id: root

    property ShaderEffectSource shaderEffectSource

    // Shader properties
    property var source: shaderEffectSource
    property real colorFactor: 0 // 1 = grayscale, 0 = full color

    //blending: false
    vertexShader: "qrc:shadereffects/vertexshaders/grayscale.frag"
    fragmentShader: "qrc:shadereffects/fragmentshaders/grayscale.frag"
}
