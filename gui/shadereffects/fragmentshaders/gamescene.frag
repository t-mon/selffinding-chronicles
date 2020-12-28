uniform lowp float qt_Opacity;
varying lowp vec2 coordinate;
uniform sampler2D world;
uniform sampler2D light;

// ###################################################
// Shader parameters
// ###################################################

// Grayscale (1 = full grayscale, 0 = full color)
uniform lowp float grayscaleFactor;

uniform lowp float brightness;
uniform lowp float contrast;
uniform lowp float gamma;

// Ambient
uniform lowp float ambientBrightness;
uniform lowp vec4 ambientLightColor;

// Stoned effect
uniform lowp float amplitude;
uniform lowp float frequency;
uniform lowp float time;


// ###################################################
// Function declarations
// ###################################################

vec3 convertGrayscale(vec3 color, float colorFactor);
vec3 brightnessContrastCorrection(vec3 color, float brightness, float contrast);
vec3 gammaCorrection(vec3 color, float gamma);

vec3 rgb2hsb(vec3 color);
vec3 hsb2rgb(vec3 color);


float blendAdd(float base, float blend);
vec3 blendAdd(vec3 base, vec3 blend);
vec3 blendAdd(vec3 base, vec3 blend, float opacity);

vec3 blendMultiply(vec3 base, vec3 blend);
vec3 blendMultiply(vec3 base, vec3 blend, float opacity);

vec3 blendAverage(vec3 base, vec3 blend);
vec3 blendAverage(vec3 base, vec3 blend, float opacity);

float blendColorBurn(float base, float blend);
vec3 blendColorBurn(vec3 base, vec3 blend);
vec3 blendColorBurn(vec3 base, vec3 blend, float opacity);

float blendOverlay(float base, float blend);
vec3 blendOverlay(vec3 base, vec3 blend);
vec3 blendOverlay(vec3 base, vec3 blend, float opacity);

float blendLighten(float base, float blend);
vec3 blendLighten(vec3 base, vec3 blend);
vec3 blendLighten(vec3 base, vec3 blend, float opacity);

float blendSoftLight(float base, float blend);
vec3 blendSoftLight(vec3 base, vec3 blend);
vec3 blendSoftLight(vec3 base, vec3 blend, float opacity);



// ###################################################
// Main
// ###################################################

void main(void)
{
    // Perform stoned effect
    vec4 worldPixel = texture2D(world, coordinate);
    vec4 lightPixel = texture2D(light, coordinate);

    // Perform stoned animation
    if (amplitude != 0.0) {
        vec2 stonedPosition = sin(time + frequency * coordinate);
        worldPixel = texture2D(world, coordinate + amplitude * vec2(stonedPosition.y, -stonedPosition.x));
        lightPixel = texture2D(light, coordinate + amplitude * vec2(stonedPosition.y, -stonedPosition.x));
    }

    // Perform gray scale convertion
    if (grayscaleFactor != 0.0) {
        worldPixel = vec4(convertGrayscale(worldPixel.rgb, grayscaleFactor), worldPixel.a);
    }

    // Perform ambient light blending
    if (ambientLightColor.a != 0.0) {
        worldPixel = vec4(blendSoftLight(worldPixel.rgb, ambientLightColor.rgb, ambientLightColor.a), worldPixel.a);
    }

    // Perform light color blending if there is light
    if (lightPixel.a != 0.0) {
        vec3 lightColor = lightPixel.rgb;
        worldPixel = vec4(blendSoftLight(worldPixel.rgb, lightColor, lightPixel.a), worldPixel.a);
    }

    // Apply gamma correction
    worldPixel = vec4(gammaCorrection(worldPixel.rgb, gamma), worldPixel.a);

    // Apply brightness/contrast correction
    //worldPixel = vec4(brightnessContrastCorrection(worldPixel.rgb, brightness, contrast), worldPixel.a);

    // Perform ambient brightness
    if (ambientBrightness != 1.0) {
        float brightness = ambientBrightness;
        brightness = mix(ambientBrightness, 1.0, lightPixel.a);

        vec3 hsbWorldColor = rgb2hsb(worldPixel.rgb);
        hsbWorldColor.b = hsbWorldColor.b * brightness;
        worldPixel = vec4(hsb2rgb(hsbWorldColor), worldPixel.a);
    }

    gl_FragColor = worldPixel * qt_Opacity;
}


// ###################################################
// Function implementations
// ###################################################

// Convert to grayscale
vec3 convertGrayscale(vec3 color, float colorFactor)
{
    float grey =  0.21 * color.r + 0.71 * color.g + 0.07 * color.b;
    return color.rgb * (1.0 - colorFactor) + (grey * colorFactor);
}

// Aply brightness contrast correction
vec3 brightnessContrastCorrection(vec3 color, float brightness, float contrast)
{
    return (color - 0.5) * contrast + 0.5 + brightness;
}

// Apply gamma correction
vec3 gammaCorrection(vec3 color, float gamma)
{
    return pow(color, vec3(1.0 / gamma));
}

// Convert RGB <--> HSL
vec3 rgb2hsb(vec3 color)
{
    vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
    vec4 p = mix(vec4(color.bg, K.wz), vec4(color.gb, K.xy), step(color.b, color.g));
    vec4 q = mix(vec4(p.xyw, color.r), vec4(color.r, p.yzx), step(p.x, color.r));
    float d = q.x - min(q.w, q.y);
    float e = 1.0e-10;
    return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
}

vec3 hsb2rgb(vec3 color)
{
    vec3 rgb = clamp(abs(mod(color.x * 6.0 + vec3(0.0, 4.0, 2.0), 6.0) - 3.0) -1.0, 0.0, 1.0);
    rgb = rgb * rgb * (3.0 - 2.0 * rgb);
    return color.z * mix(vec3(1.0), rgb, color.y);
}

// Soft light blending
float blendSoftLight(float base, float blend)
{
    if (blend < 0.5) {
        return 2.0 * base * blend + base * base * (1.0 - 2.0 * blend);
    } else {
        return sqrt(base) * (2.0 * blend - 1.0) + 2.0 * base * (1.0 - blend);
    }
}

vec3 blendSoftLight(vec3 base, vec3 blend)
{
    return vec3(blendSoftLight(base.r, blend.r),
                blendSoftLight(base.g, blend.g),
                blendSoftLight(base.b, blend.b));
}

vec3 blendSoftLight(vec3 base, vec3 blend, float opacity)
{
    return (blendSoftLight(base, blend) * opacity + base * (1.0 - opacity));
}


// Add blending
float blendAdd(float base, float blend)
{
    return min(base + blend, 1.0);
}

vec3 blendAdd(vec3 base, vec3 blend)
{
    return min(base + blend, vec3(1.0));
}

vec3 blendAdd(vec3 base, vec3 blend, float opacity)
{
    return (blendAdd(base, blend) * opacity + base * (1.0 - opacity));
}


// Color burn blending
float blendColorBurn(float base, float blend)
{
    if (blend == 0.0) {
        return blend;
    } else {
        return max((1.0 - ((1.0 - base) / blend)), 0.0);
    }
}

vec3 blendColorBurn(vec3 base, vec3 blend)
{
    return vec3(blendColorBurn(base.r, blend.r),
                blendColorBurn(base.g, blend.g),
                blendColorBurn(base.b, blend.b));
}

vec3 blendColorBurn(vec3 base, vec3 blend, float opacity)
{
    return blendColorBurn(base, blend) * opacity + base * (1.0 - opacity);
}


// Overlay blending
float blendOverlay(float base, float blend)
{
    if (base < 0.5) {
        return 2.0 * base * blend;
    } else {
        return 1.0 - 2.0 * (1.0 - base) * (1.0 - blend);
    }
}

vec3 blendOverlay(vec3 base, vec3 blend)
{
    return vec3(blendOverlay(base.r, blend.r),
                blendOverlay(base.g, blend.g),
                blendOverlay(base.b, blend.b));
}

vec3 blendOverlay(vec3 base, vec3 blend, float opacity)
{
    return (blendOverlay(base, blend) * opacity + base * (1.0 - opacity));
}

// Lighten blending
float blendLighten(float base, float blend)
{
    return max(blend, base);
}

vec3 blendLighten(vec3 base, vec3 blend)
{
    return vec3(blendLighten(base.r, blend.r),
                blendLighten(base.g, blend.g),
                blendLighten(base.b, blend.b));
}

vec3 blendLighten(vec3 base, vec3 blend, float opacity)
{
    return (blendLighten(base, blend) * opacity + base * (1.0 - opacity));
}

// Multiply blending
vec3 blendMultiply(vec3 base, vec3 blend)
{
    return base * blend;
}

vec3 blendMultiply(vec3 base, vec3 blend, float opacity)
{
    return (blendMultiply(base, blend) * opacity + base * (1.0 - opacity));
}

// Average blending
vec3 blendAverage(vec3 base, vec3 blend)
{
    return (base + blend) / 2.0;
}

vec3 blendAverage(vec3 base, vec3 blend, float opacity)
{
    return (blendAverage(base, blend) * opacity + base * (1.0 - opacity));
}
