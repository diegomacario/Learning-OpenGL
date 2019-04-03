#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out VS_OUT {
    vec3 normal;
} vs_out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    // We transform the normal to clip space coordinates because the geometry shader receives the vertex positions in clip space coordinates
    mat3 normalMatrix = mat3(transpose(inverse(view * model)));
    vs_out.normal = vec3(projection * vec4(normalMatrix * aNormal, 0.0));

    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
