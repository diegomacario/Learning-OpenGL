#version 330 core

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normal;

out VertexData
{
   vec3 worldPos;
   vec3 worldNormal;
   vec2 texCoords;
} o;

void main()
{
   o.worldPos    = vec3(model * vec4(inPos, 1.0));
   o.worldNormal = normalize(normal * inNormal);
   o.texCoords   = inTexCoords;

   gl_Position = projection * view * vec4(o.worldPos, 1.0);
}
