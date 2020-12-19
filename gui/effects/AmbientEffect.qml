import QtQuick 2.9
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.0

ShaderEffect {
    id: root

    property ShaderEffectSource shaderEffectSource

    // Shader properties
    property var source: shaderEffectSource
    property color ambientLightColor: Qt.rgba(0, 0, 0, 0)

    fragmentShader: "
        varying highp vec2 qt_TexCoord0;
        uniform sampler2D source;
        uniform lowp float qt_Opacity;

        uniform highp vec4 ambientLightColor;

        float blendSoftLight(float base, float blend);
        vec3 blendSoftLight(vec3 base, vec3 blend);
        vec3 blendSoftLight(vec3 base, vec3 blend, float opacity);

        void main(void) {
            if (ambientLightColor.a == 0.0) {
                gl_FragColor = texture2D(source, qt_TexCoord0) * qt_Opacity;
                return;
            }

            vec4 pixel = texture2D(source, qt_TexCoord0);
            vec3 ambientColor = blendSoftLight(pixel.rgb, ambientLightColor.rgb, ambientLightColor.a);
            gl_FragColor = vec4(ambientColor.rgb, 1.0) * qt_Opacity;
        }

        float blendSoftLight(float base, float blend) {
            if (blend < 0.5) {
                return 2.0 * base * blend + base * base * (1.0 - 2.0 * blend);
            } else {
                return sqrt(base) * (2.0 * blend - 1.0) + 2.0 * base * (1.0 - blend);
            }
        }

        vec3 blendSoftLight(vec3 base, vec3 blend) {
            return vec3(blendSoftLight(base.r, blend.r), blendSoftLight(base.g, blend.g), blendSoftLight(base.b, blend.b));
        }

        vec3 blendSoftLight(vec3 base, vec3 blend, float opacity) {
            return (blendSoftLight(base, blend) * opacity + base * (1.0 - opacity));
        }
    "
}
