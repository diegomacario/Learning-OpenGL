#version 330 core

layout (location = 0) in vec4 inPosAndTexCoords; // The first two elements correspond to the position, while the second two elements corresponds to the texture coordinates

uniform Uniforms
{
   mat4 model;
   mat4 projection;
} u;

out VertexData
{
   vec2 texCoords;
} o;

void main()
{
   o.texCoords = inPosAndTexCoords.zw;
   gl_Position = u.projection * u.model * vec4(inPosAndTexCoords.xy, 0.0, 1.0);
}
