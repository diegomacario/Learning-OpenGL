#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

//uniform sampler2D texture_diffuse1;
//uniform sampler2D texture_specular1;
//uniform sampler2D texture_normal1;
uniform samplerCube skybox;
uniform vec3 cameraPos;

void main()
{
   vec3 I    = FragPos - cameraPos;
   vec3 R    = reflect(I, normalize(Normal));
   FragColor = vec4(texture(skybox, R).rgb, 1.0);
}
