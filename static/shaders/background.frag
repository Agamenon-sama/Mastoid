#version 440

layout(location = 0) in vec2 texCoord;
layout(location = 0) out vec4 fragColor;

layout(binding = 1) uniform sampler2D source;

//uniform sampler2D qt_Texture0;
//varying highp vec4 qt_TexCoord0;

//layout(std140, binding = 0) uniform buf {
//    float uAlpha;
//};

void main(void)
{
    // offset matrix
    float offset = 1.f / 100.f; // todo: investigate this. maybe I can get a better blur if I make this more dynamic
    vec2 offsets[9] = vec2[] (
            vec2(-offset, offset),  vec2(0.f, offset),  vec2(offset, offset),
            vec2(-offset, 0.f),     vec2(0.f, 0.f),     vec2(offset, 0.f),
            vec2(-offset, -offset), vec2(0.f, -offset), vec2(offset, -offset)
    );

    // fast blur kernel
    float kernel[9] = float[] (
            1.f / 16.f, 2.f / 16.f, 1.f / 16.f,
            2.f / 16.f, 4.f / 16.f, 2.f / 16.f,
            1.f / 16.f, 2.f / 16.f, 1.f / 16.f
    );

    vec3 col = vec3(0);
    for (int i = 0; i < 9; i++) {
        col += kernel[i] * vec3(texture(source, texCoord + offsets[i]));
    }

    // 80% overlay, 15% blurred texture, 5% original texture to rectify the ugly blur quality
    fragColor = 0.8f * vec4(0.3f, 0.3f, 0.35f, 1.f) + 0.15f * vec4(col, 1.f) + 0.05f * texture(source, texCoord);
}
