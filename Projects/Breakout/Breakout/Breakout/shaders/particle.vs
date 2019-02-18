#version 330 core
layout (location = 0) in vec4 vertex; // <vec2 position, vec2 texCoords>

out vec2 TexCoords;
out vec4 ParticleColor;

uniform vec4 color;
uniform vec2 translation;
uniform mat4 projection;

void main()
{
    TexCoords = vertex.zw;
    ParticleColor = color;

    float scale = 10.0f;
    gl_Position = projection * vec4((vertex.xy * scale) + translation, 0.0, 1.0);
}
