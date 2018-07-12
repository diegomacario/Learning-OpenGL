#version 330 core
out vec4 FragColor;

struct Material
{
   sampler2D diffuse;
   sampler2D specular;
   float     shininess;
};

struct DirLight
{
   vec3 direction;

   vec3 ambient;
   vec3 diffuse;
   vec3 specular;
};

struct PointLight
{
   vec3 position;

   vec3 ambient;
   vec3 diffuse;
   vec3 specular;

   float constant;
   float linear;
   float quadratic;
};

struct SpotLight
{
   vec3  position;
   vec3  direction;
   float cutOff;
   float outerCutOff;

   vec3 ambient;
   vec3 diffuse;
   vec3 specular;

   float constant;
   float linear;
   float quadratic;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3     viewPos;
uniform Material material;

#define NR_POINT_LIGHTS 4
uniform DirLight   dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight  spotLight;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
   vec3 norm    = normalize(Normal);
   vec3 viewDir = normalize(viewPos - FragPos);

   vec3 result = CalcDirLight(dirLight, norm, viewDir);

   for(int i = 0; i < NR_POINT_LIGHTS; i++)
   {
      result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
   }

   result += CalcSpotLight(spotLight, norm, FragPos, viewDir);

   FragColor = vec4(result, 1.0);
}

// Assume that the normal and the view direction are normalized
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
   // Ambient
   vec3 ambient     = light.ambient * texture(material.diffuse, TexCoords).rgb;

   // Diffuse
   vec3  lightDir   = normalize(-light.direction);
   float diff       = max(dot(lightDir, normal), 0.0);
   vec3  diffuse    = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;

   // Specular
   vec3  reflectDir = reflect(-lightDir, normal);
   float spec       = pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);
   vec3  specular   = light.specular * spec * texture(material.specular, TexCoords).rgb;

   return (ambient + diffuse + specular);
}

// Assume that the normal and the view direction are normalized
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
   // Ambient
   vec3 ambient      = light.ambient * texture(material.diffuse, TexCoords).rgb;

   // Diffuse
   vec3  lightDir    = normalize(light.position - fragPos);
   float diff        = max(dot(lightDir, normal), 0.0);
   vec3  diffuse     = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;

   // Specular
   vec3 reflectDir   = reflect(-lightDir, normal);
   float spec        = pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);
   vec3 specular     = light.specular * spec * texture(material.specular, TexCoords).rgb;

   // Attenuation
   float distance    = length(light.position - fragPos);
   float attenuation = 1.0 / (light.constant + (light.linear * distance) + (light.quadratic * distance * distance));

   ambient           *= attenuation;
   diffuse           *= attenuation;
   specular          *= attenuation;

   return (ambient + diffuse + specular);
}

// Assume that the normal and the view direction are normalized
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
   // Ambient
   vec3  ambient     = light.ambient * texture(material.diffuse, TexCoords).rgb;

   // Diffuse
   vec3  lightDir    = normalize(light.position - fragPos);
   float diff        = max(dot(lightDir, normal), 0.0);
   vec3  diffuse     = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;

   // Specular
   vec3  reflectDir  = reflect(-lightDir, normal);
   float spec        = pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);
   vec3  specular    = light.specular * spec * texture(material.specular, TexCoords).rgb;

   // Attenuation
   float distance    = length(light.position - fragPos);
   float attenuation = 1.0 / (light.constant + (light.linear * distance) + (light.quadratic * distance * distance));

   // Do not attenuate the ambient light. Otherwise, the objects inside the spotlight will be darker than
   // the objects outside of it at large distances.
   diffuse          *= attenuation;
   specular         *= attenuation;

   // Smooth edges of the spotlight
   // Cosine of the angle between the direction of the spotlight and the direction of the light ray to the fragment
   float theta       = dot(lightDir, normalize(-light.direction));
   // Difference between cos(cutOff) and cos(outerCutOff)
   float epsilon     = light.cutOff - light.outerCutOff;
   // The intensity is greater than 1.0 when inside the inner cone, between 1.0 and 0.0 when between the two cones,
   // and smaller than zero when outside the outer cone
   float intensity   = clamp(((theta - light.outerCutOff) / epsilon), 0.0, 1.0);

   // Do not scale the ambient light with the intensity, so that we have a little bit of light outside of the spotlight
   diffuse          *= intensity;
   specular         *= intensity;

   return (ambient + diffuse + specular);
}
