#version 440

layout(location = 0) in vec2 texCoord;
layout(location = 0) out vec4 fragColor;

layout(binding = 1) uniform sampler2D source;

layout(std140, binding = 0) uniform buf {
    mat4 qt_Matrix;
    float qt_opacity;
};

void main(void) {
    vec3 col = vec3(texture(source, texCoord));
    col = vec3(0.2126 * col.r + 0.7152 * col.g + 0.0722 * col.b);

    // 20% overlay, 35% grayscale texture, 45% original texture
    fragColor = 0.2f * vec4(0.3f, 0.3f, 0.35f, 1.f) + 0.35f * vec4(col, 1.f) + 0.45f * texture(source, texCoord);
}
