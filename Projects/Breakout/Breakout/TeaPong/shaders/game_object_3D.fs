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

struct MaterialConstants
{
   vec3  ambient;
   vec3  diffuse;
   vec3  specular;
   vec3  emissive;
   float shininess;
};

#define MAX_NUMBER_OF_POINT_LIGHTS 4
uniform PointLight pointLights[MAX_NUMBER_OF_POINT_LIGHTS];
uniform int numPointLightsInScene;

uniform vec3 cameraPos;

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
   vec3 ambient      = materialConstants.ambient * attenuation;

   // Diffuse
   vec3  lightDir    = normalize(light.worldPos - i.worldPos);
   vec3  diffuse     = max(dot(lightDir, i.worldNormal), 0.0) * materialConstants.diffuse * light.color * attenuation;

   // Specular
   vec3 reflectedDir = reflect(-lightDir, i.worldNormal);
   vec3 specular     = pow(max(dot(reflectedDir, viewDir), 0.0), shininess) * materialConstants.specular * light.color * attenuation;

   // Emissive
   vec3 emissive     = materialConstants.emissive;

   return (ambient + diffuse + specular + emissive);
}
