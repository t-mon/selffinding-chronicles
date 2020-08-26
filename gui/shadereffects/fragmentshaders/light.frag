varying highp vec2 qt_TexCoord0;
uniform sampler2D source;
uniform lowp float qt_Opacity;

uniform vec2 playerPosition;
uniform vec2 screenSize; // px
uniform float screenRatio;
uniform float lightRadius; // gridSize

vec3 rgb2hsb(vec3 c) {
    vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
    vec4 p = mix(vec4(c.bg, K.wz),
                 vec4(c.gb, K.xy),
                 step(c.b, c.g));
    vec4 q = mix(vec4(p.xyw, c.r),
                 vec4(c.r, p.yzx),
                 step(p.x, c.r));
    float d = q.x - min(q.w, q.y);
    float e = 1.0e-10;
    return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)),
                d / (q.x + e),
                q.x);
}

vec3 hsb2rgb(vec3 c) {
    vec3 rgb = clamp(abs(mod(c.x*6.0+vec3(0.0,4.0,2.0), 6.0)-3.0)-1.0, 0.0, 1.0 );
    rgb = rgb*rgb*(3.0 - 2.0 * rgb);
    return c.z * mix(vec3(1.0), rgb, c.y);
}

bool pixelInRadius(vec2 coordinate) {
    vec2 pixelVector = playerPosition - coordinate;
    pixelVector.x = pixelVector.x * screenRatio;
    mediump float distance = sqrt(pixelVector.x * pixelVector.x + pixelVector.y * pixelVector.y);
    return distance < lightRadius;
}

void main(void) {
    lowp vec4 texture = texture2D(source, qt_TexCoord0);
    vec3 hsbColor = rgb2hsb(texture.rgb);
    if (pixelInRadius(qt_TexCoord0)) {
        hsbColor.b = hsbColor.s * 1.1;
        vec3 rgbColor = hsb2rgb(hsbColor);
        gl_FragColor = vec4(rgbColor, texture.a) * qt_Opacity;
        gl_FragColor = vec4(vec3(texture.rgb), texture.a) * qt_Opacity;
    } else {
        hsbColor.b = hsbColor.b * 0.5;
        vec3 rgbColor = hsb2rgb(hsbColor);
        gl_FragColor = vec4(rgbColor, texture.a) * qt_Opacity;
    }
}
