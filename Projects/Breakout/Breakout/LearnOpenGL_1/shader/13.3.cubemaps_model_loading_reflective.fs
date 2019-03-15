#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform sampler2D texture_diffuse1;
//uniform sampler2D texture_specular1;
//uniform sampler2D texture_normal1;

void main()
{
   vec3 norm    = normalize(Normal);
   FragColor = texture(texture_diffuse1, TexCoords);
}
