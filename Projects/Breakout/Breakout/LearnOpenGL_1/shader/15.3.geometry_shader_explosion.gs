#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT
{
    vec2 texCoords; // Input is array of vec2 (one for each vertex)
} gs_in[];

out vec2 TexCoords; // Output is a single vec2 (its value is stored by each vertex when we call EmitVertex)

uniform float time;

vec4 explode(vec4 position, vec3 normal)
{
    float magnitude = 2.0;
    vec3 direction = normal * ((sin(time) + 1.0) / 2.0) * magnitude; // ((sin(time) + 1.0) / 2.0) produces values in the range [0.0, 1.0]
                                                                     // We don't just use sin(time) because that would produce values in the range [-1.0, 1.0], which would cause the object to implode too
    return position + vec4(direction, 0.0);
}

vec3 GetNormal()
{
    vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
    vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
    return normalize(cross(a, b));
}

void main()
{
    vec3 normal = GetNormal();

    gl_Position = explode(gl_in[0].gl_Position, normal);
    TexCoords = gs_in[0].texCoords;
    EmitVertex();
    gl_Position = explode(gl_in[1].gl_Position, normal);
    TexCoords = gs_in[1].texCoords;
    EmitVertex();
    gl_Position = explode(gl_in[2].gl_Position, normal);
    TexCoords = gs_in[2].texCoords;
    EmitVertex();
    EndPrimitive();
}
