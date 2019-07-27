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

uniform sampler2D ambientTex;
uniform sampler2D emissiveTex;
uniform sampler2D diffuseTex;
uniform sampler2D specularTex;

struct MaterialTextureAvailabilities
{
   int ambientTexIsAvailable;
   int emissiveTexIsAvailable;
   int diffuseTexIsAvailable;
   int specularTexIsAvailable;
};

uniform MaterialTextureAvailabilities materialTextureAvailabilities;

struct MaterialConstants
{
   vec3  ambient;
   vec3  emissive;
   vec3  diffuse;
   vec3  specular;
   float shininess;
};

uniform MaterialConstants materialConstants;

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
   vec3 ambient      =   (vec3(texture(ambientTex, i.texCoords)) * attenuation) *  materialTextureAvailabilities.ambientTexIsAvailable
                       - (materialConstants.ambient               * attenuation) * (materialTextureAvailabilities.ambientTexIsAvailable - 1);

   // Emissive
   vec3 emissive     =   vec3(texture(emissiveTex, i.texCoords)) *  materialTextureAvailabilities.emissiveTexIsAvailable
                       - materialConstants.emissive               * (materialTextureAvailabilities.emissiveTexIsAvailable - 1);

   // Diffuse
   vec3  lightDir    = normalize(light.worldPos - i.worldPos);
   vec3  diffuse     =   (max(dot(lightDir, i.worldNormal), 0.0) * vec3(texture(diffuseTex, i.texCoords)) * light.color * attenuation) *  materialTextureAvailabilities.diffuseTexIsAvailable
                       - (max(dot(lightDir, i.worldNormal), 0.0) * materialConstants.diffuse               * light.color * attenuation) * (materialTextureAvailabilities.diffuseTexIsAvailable - 1);

   // Specular
   vec3 reflectedDir = reflect(-lightDir, i.worldNormal);
   vec3 specular     =   (pow(max(dot(reflectedDir, viewDir), 0.0), materialConstants.shininess) * vec3(texture(specularTex, i.texCoords)) * light.color * attenuation) *  materialTextureAvailabilities.specularTexIsAvailable
                       - (pow(max(dot(reflectedDir, viewDir), 0.0), materialConstants.shininess) * materialConstants.specular               * light.color * attenuation) * (materialTextureAvailabilities.specularTexIsAvailable - 1);

   return (ambient + diffuse + specular + emissive);
}
