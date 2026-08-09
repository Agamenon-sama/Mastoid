#version 440

layout(location = 0) in vec2 texCoord;
layout(location = 0) out vec4 fragColor;

layout(binding = 1) uniform sampler2D source;

layout(std140, binding = 0) uniform buf {
    mat4 qt_Matrix;
    float qt_Opacity;
    float u_bass;
    float u_treble;
    float u_beat;
    float u_time;
    bool u_audioEffect;
};

#define SHAKE_INTENSITY 0.01f
#define ABERRATION_INTENSITY 0.03f
#define BLUR_INTENSITY 0.05f
#define VIGNETTE_INTENSITY 15.f

vec2 hash(float n) {
    vec2 p = fract(vec2(n * 5.398f, n * 5.442f));
    p += dot(p.yx, p.xy + vec2(25.967f, 17.312f));
    return fract(vec2(p.x * p.y * 95.433f, p.x * p.y * 97.59f)) * 2.f - 1.f;
}

vec4 sampleWithAberration(sampler2D s, vec2 uv, float aberrationFactor) {
    vec4 col = texture(source, uv);
    col.r = texture(s, vec2(uv.x - aberrationFactor, uv.y)).r;
    col.b = texture(s, vec2(uv.x + aberrationFactor, uv.y)).b;

    return col;
}

void main(void) {
    vec4 col = vec4(1.f);
    vec4 overlay = vec4(0.3f, 0.3f, 0.35f, 1.f);

    vec2 uv = texCoord + (u_audioEffect ? SHAKE_INTENSITY * u_treble * hash(u_time) : vec2(0.f));

    vec4 grayScale = texture(source, uv);
    grayScale = vec4(vec3(0.2126f * grayScale.r + 0.7152f * grayScale.g + 0.0722f * grayScale.b), 1.f);
    
    float aberrationFactor = ABERRATION_INTENSITY * u_beat;

    vec4 colorTex = vec4(1.f);
    if (u_audioEffect) {
        colorTex = sampleWithAberration(source, uv, aberrationFactor) * 0.5f;

        vec2 blurDir = vec2(uv - 0.5f);

        for(int i=1; i<=4; i++) {
            float fi = float(i);
            vec2 offset = blurDir * fi * u_bass * BLUR_INTENSITY;

            colorTex += sampleWithAberration(source, uv + offset, aberrationFactor) * 0.125f;
            colorTex += sampleWithAberration(source, uv - offset, aberrationFactor) * 0.125f;
        }
    }
    else {
        colorTex = texture(source, uv);
    }

    colorTex.a = 1.f;

    // 20% overlay, 35% grayscale texture, 45% color texture
    col = 0.2f * overlay + 0.35f * grayScale + 0.45f * colorTex;

    if (u_audioEffect) {
        float vigFactor = u_beat;
        float edgeFactor = texCoord.x * (1.f - texCoord.x);
        float vig = pow(edgeFactor * VIGNETTE_INTENSITY, vigFactor);
        vig = clamp(1.f - vig, 0.f, 1.f);
        col.rgb = mix(col.rgb, vec3(1.f), vec3(vig));
    }

    
    fragColor = col * qt_Opacity;
}
