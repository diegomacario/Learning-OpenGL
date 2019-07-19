#version 330 core

in VertexData
{
   vec2 texCoords;
} i;

uniform Uniforms
{
   sampler2D image;
} u;

out vec4 fragColor;

void main()
{
   fragColor = texture(u.image, i.texCoords);
}
