#version 330 core

in VertexData
{
   vec3 worldPosition;
   vec3 worldNormal;
   vec2 texCoords;
} i;

uniform sampler2D emissiveTex0;

out vec4 fragColor;

void main()
{
   fragColor = texture(emissiveTex0, i.texCoords);
   //fragColor = vec4(1.0, 0.0, 1.0, 1.0);
}
