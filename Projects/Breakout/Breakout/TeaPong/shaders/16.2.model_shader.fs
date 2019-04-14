#version 330 core

out vec4 FragColor;

struct PointLight
{
   vec3  position;
   vec3  color;
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
//uniform sampler2D texture_diffuse1;
//uniform sampler2D texture_specular1;
//uniform sampler2D texture_normal1;

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
   vec3 norm    = normalize(Normal);
   vec3 viewDir = normalize(viewPos - FragPos);
   vec3 result  = CalcPointLight(pointLight, norm, FragPos, viewDir);

   FragColor = vec4(result, 1.0);
   //FragColor = vec4(0.3, 0.5, 0.2, 1.0);
}

// Assume that the normal and the view direction are normalized
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
   vec3 teapotAmbient = vec3(0.0, 0.0, 0.125);
   vec3 teapotDiffuse = vec3(0.5, 0.0, 0.5);
   vec3 teapotSpecular = vec3(0.5, 0.5, 0.5);
   float teapotShininess = 10.0;

   // Ambient
   vec3 ambient      = teapotAmbient;

   // Diffuse
   vec3  lightDir    = normalize(light.position - fragPos);
   float diff        = max(dot(lightDir, normal), 0.0);
   vec3  diffuse     = light.color * diff * teapotDiffuse;

   // Specular
   vec3 reflectDir   = reflect(-lightDir, normal);
   float spec        = pow(max(dot(reflectDir, viewDir), 0.0), teapotShininess);
   vec3 specular     = light.color * spec * teapotSpecular;

   // Attenuation
   float distance    = length(light.position - fragPos);
   float attenuation = 1.0 / (light.constant + (light.linear * distance) + (light.quadratic * distance * distance));

   //ambient           *= attenuation;
   //diffuse           *= attenuation;
   //specular          *= attenuation;

   return (ambient + diffuse + specular);
}
