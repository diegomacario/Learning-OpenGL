#version 330 core

in VertexData
{
   vec3 worldPos;
   vec3 worldNormal;
   vec2 texCoords;
} i;

uniform vec3  pl_worldPos;
uniform vec3  pl_color;
uniform float pl_constantAtt;
uniform float pl_linearAtt;
uniform float pl_quadraticAtt;

uniform vec3 cameraPos;

//uniform sampler2D ambientTex;
//uniform sampler2D emissiveTex;
//uniform sampler2D diffuseTex;
//uniform sampler2D specularTex;

//struct MaterialTextureAvailabilities
//{
//   int ambientTexIsAvailable;
//   int emissiveTexIsAvailable;
//   int diffuseTexIsAvailable;
//   int specularTexIsAvailable;
//};

//uniform MaterialTextureAvailabilities materialTextureAvailabilities;

uniform vec3  ambientConst;
uniform vec3  emissiveConst;
uniform vec3  diffuseConst;
uniform vec3  specularConst;
uniform float shininessConst;

out vec4 fragColor;

vec3 calculateContributionOfPointLight(vec3 viewDir);

void main()
{
   vec3 viewDir = normalize(cameraPos - i.worldPos);

   vec3 color = calculateContributionOfPointLight(viewDir);

   fragColor = vec4(color, 1.0);
}

vec3 calculateContributionOfPointLight(vec3 viewDir)
{
   // Attenuation
   float distance    = length(pl_worldPos - i.worldPos);
   float attenuation = 1.0 / (pl_constantAtt + (pl_linearAtt * distance) + (pl_quadraticAtt * distance * distance));

   // Ambient
   // TODO: Do you really want the ambient light to be attenuated?
   vec3 ambient      =   (ambientConst * attenuation);

   // Emissive
   vec3 emissive     =   emissiveConst;

   // Diffuse
   vec3  lightDir    =   normalize(pl_worldPos - i.worldPos);
   vec3  diff        =   max(dot(lightDir, i.worldNormal), 0.0) * pl_color * attenuation;
   vec3  diffuse     =   (diff * diffuseConst);

   // Specular
   vec3 reflectedDir =   reflect(-lightDir, i.worldNormal);
   vec3 spec         =   pow(max(dot(reflectedDir, viewDir), 0.0), shininessConst) * pl_color * attenuation;
   vec3 specular     =   (spec * specularConst);

   return (ambient + diffuse + specular + emissive);
}
