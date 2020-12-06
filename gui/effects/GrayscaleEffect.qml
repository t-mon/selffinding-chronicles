import QtQuick 2.9
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.0

ShaderEffect {
    id: root

    property ShaderEffectSource shaderEffectSource
    property bool effectEnabled: false
    property var source: shaderEffectSource

    blending: false
    vertexShader: root.effectEnabled ? "qrc:shadereffects/vertexshaders/grayscale.frag" : "qrc:shadereffects/vertexshaders/default.frag"
    fragmentShader: root.effectEnabled ? "qrc:shadereffects/fragmentshaders/grayscale.frag" : "qrc:shadereffects/fragmentshaders/default.frag"
}
