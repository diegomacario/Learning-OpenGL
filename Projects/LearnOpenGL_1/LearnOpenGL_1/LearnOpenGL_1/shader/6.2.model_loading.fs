#version 330 core
out vec4 FragColor;

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

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3       viewPos;
uniform PointLight pointLight;

uniform float     shininess;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
//uniform sampler2D texture_normal1;

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
   vec3 norm    = normalize(Normal);
   vec3 viewDir = normalize(viewPos - FragPos);
   vec3 result  = CalcPointLight(pointLight, norm, FragPos, viewDir);

   FragColor = vec4(result, 1.0);
}

// Assume that the normal and the view direction are normalized
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
   // Ambient
   vec3 ambient      = light.ambient * texture(texture_diffuse1, TexCoords).rgb;

   // Diffuse
   vec3  lightDir    = normalize(light.position - fragPos);
   float diff        = max(dot(lightDir, normal), 0.0);
   vec3  diffuse     = light.diffuse * diff * texture(texture_diffuse1, TexCoords).rgb;

   // Specular
   vec3 reflectDir   = reflect(-lightDir, normal);
   float spec        = pow(max(dot(reflectDir, viewDir), 0.0), shininess);
   vec3 specular     = light.specular * spec * texture(texture_specular1, TexCoords).rgb;

   // Attenuation
   float distance    = length(light.position - fragPos);
   float attenuation = 1.0 / (light.constant + (light.linear * distance) + (light.quadratic * distance * distance));

   ambient           *= attenuation;
   diffuse           *= attenuation;
   specular          *= attenuation;

   return (ambient + diffuse + specular);
}
