uniform sampler2D source;
uniform lowp float qt_Opacity;
varying highp vec2 coordinate;

void main() {
    lowp vec4 pixel = texture2D(source, coordinate);
    gl_FragColor = vec4(pixel.rgb, pixel.a) * qt_Opacity;
}
