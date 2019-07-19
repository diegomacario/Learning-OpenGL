#version 330 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTexCoords;

uniform Uniforms
{
   mat4 model;
   mat4 view;
   mat4 projection;
} u;

out VertexData
{
   vec3 worldPosition;
   vec3 worldNormal;
   vec2 texCoords;
} o;

void main()
{
   o.worldPosition = vec3(u.model * vec4(inPosition, 1.0));
   o.worldNormal   = mat3(transpose(inverse(u.model))) * inNormal; // We only take upper-left 3x3 because normals should not be affected by translations, and we take the transpose of the inverse to handle non-uniform scales
   o.texCoords     = inTexCoords;

   gl_Position   = u.projection * u.view * vec4(o.worldPosition, 1.0);
}
