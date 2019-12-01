varying highp vec2 qt_TexCoord0;

uniform sampler2D source;
uniform lowp float qt_Opacity;
uniform lowp float blueChannel;

void main() {
    gl_FragColor = texture2D(source, qt_TexCoord0) * vec4(0.5, 0.5, blueChannel, 1.0) * qt_Opacity;
}
