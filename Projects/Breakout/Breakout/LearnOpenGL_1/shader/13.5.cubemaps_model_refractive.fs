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
   float ratio = 1.00 / 1.52; // Refractive index of air divided by the refractive index of glass = N_src / N_dest
   vec3 I    = FragPos - cameraPos;
   vec3 R    = refract(I, normalize(Normal), ratio);
   FragColor = vec4(texture(skybox, R).rgb, 1.0);
}
