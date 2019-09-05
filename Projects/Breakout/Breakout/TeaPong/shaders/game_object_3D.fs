#version 330 core

in VertexData
{
   vec3 worldPos;
   vec3 worldNormal;
   vec2 texCoords;
} i;

struct PointLight
{
   vec3  worldPos;
   vec3  color;
   float constantAtt;
   float linearAtt;
   float quadraticAtt;
};

#define MAX_NUMBER_OF_POINT_LIGHTS 4
uniform PointLight pointLights[MAX_NUMBER_OF_POINT_LIGHTS];
uniform int numPointLightsInScene;

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

vec3 calculateContributionOfPointLight(PointLight light, vec3 viewDir);

void main()
{
   vec3 viewDir = normalize(cameraPos - i.worldPos);

   vec3 color;
   for(int i = 0; i < numPointLightsInScene; i++)
   {
      color += calculateContributionOfPointLight(pointLights[i], viewDir);
   }

   fragColor = vec4(color, 1.0);
}

vec3 calculateContributionOfPointLight(PointLight light, vec3 viewDir)
{
   // Attenuation
   float distance    = length(light.worldPos - i.worldPos);
   float attenuation = 1.0 / (light.constantAtt + (light.linearAtt * distance) + (light.quadraticAtt * distance * distance));

   // Ambient
   // TODO: Do you really want the ambient light to be attenuated?
   vec3 ambient      =   (ambientConst * attenuation);

   // Emissive
   vec3 emissive     =   emissiveConst;

   // Diffuse
   vec3  lightDir    =   normalize(light.worldPos - i.worldPos);
   vec3  diff        =   max(dot(lightDir, i.worldNormal), 0.0) * light.color * attenuation;
   vec3  diffuse     =   (diff * diffuseConst);

   // Specular
   vec3 reflectedDir =   reflect(-lightDir, i.worldNormal);
   vec3 spec         =   pow(max(dot(reflectedDir, viewDir), 0.0), shininessConst) * light.color * attenuation;
   vec3 specular     =   (spec * specularConst);

   return (ambient + diffuse + specular + emissive);
}
