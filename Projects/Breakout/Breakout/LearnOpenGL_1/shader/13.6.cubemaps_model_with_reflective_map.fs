#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform sampler2D texture_diffuse1;
//uniform sampler2D texture_specular1;
//uniform sampler2D texture_normal1;
uniform sampler2D texture_height1; // This is actually a reflection map. Assimp doesn't like reflection maps for some reason,
                                   // so it is stored as an ambient map. Search for aiTextureType_AMBIENT  in model.h to understand this.
uniform vec3 cameraPos;
uniform samplerCube skybox;

void main()
{
   vec3 I    = FragPos - cameraPos;
   vec3 R    = reflect(I, normalize(Normal));

   vec3 colorOfModel       = texture(texture_diffuse1, TexCoords).rgb;
   vec3 colorOfReflections = texture(texture_height1, TexCoords).rgb * texture(skybox, R).rgb;

   FragColor = vec4(colorOfModel + colorOfReflections, 1.0);
}
