import QtQuick 2.9
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.0

ShaderEffect {
    id: root

    property ShaderEffectSource worldShaderEffectSource
    property ShaderEffectSource lightsShaderEffectSource

    property var world: worldShaderEffectSource
    property var light: lightsShaderEffectSource

    property real ambientBrightness: 1.0
    property real alpha: 0.2

    fragmentShader: "
        varying highp vec2 qt_TexCoord0;
        uniform sampler2D world;
        uniform sampler2D light;

        uniform lowp float qt_Opacity;

        uniform highp float ambientBrightness;
        uniform highp float alpha;

        vec3 rgb2hsb(vec3 c);
        vec3 hsb2rgb(vec3 c);

        // Soft blend light
        float blendSoftLight(float base, float blend);
        vec3 blendSoftLight(vec3 base, vec3 blend);
        vec3 blendSoftLight(vec3 base, vec3 blend, float opacity);

        void main(void) {
            // Do no calculations if full brightness
            if (ambientBrightness == 1.0) {
                gl_FragColor = texture2D(world, qt_TexCoord0) * qt_Opacity;
                return;
            }

            highp vec4 worldTexture = texture2D(world, qt_TexCoord0);
            highp vec4 lightTexture = texture2D(light, qt_TexCoord0);
            highp float brightness = ambientBrightness;

            highp vec3 worldLightColor = worldTexture.rgb;
            if (lightTexture.a != 0.0) {
                vec3 lightColor = lightTexture.rgb;
                brightness = mix(ambientBrightness, 1.0, lightTexture.a);
                worldLightColor = blendSoftLight(worldTexture.rgb, lightColor.rgb, lightTexture.a);
            }

            highp vec3 hsbWorldColor = rgb2hsb(worldLightColor.rgb);
            hsbWorldColor.b = hsbWorldColor.b * brightness;
            highp vec3 finalColor = hsb2rgb(hsbWorldColor);
            gl_FragColor = vec4(finalColor, worldTexture.a) * qt_Opacity;
        }

        vec3 rgb2hsb(vec3 c) {
            highp vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
            highp vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
            highp vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));
            highp float d = q.x - min(q.w, q.y);
            highp float e = 1.0e-10;
            return highp vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
        }

        vec3 hsb2rgb(vec3 c) {
            highp vec3 rgb = clamp(abs(mod(c.x * 6.0 + vec3(0.0, 4.0, 2.0), 6.0) - 3.0) -1.0, 0.0, 1.0);
            rgb = rgb * rgb * (3.0 - 2.0 * rgb);
            return c.z * mix(vec3(1.0), rgb, c.y);
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
