#version 330 core
out vec4 FragColor;

struct Material
{
   sampler2D diffuse;
   sampler2D specular;
   float     shininess;
}; 

struct Light
{
   vec3 position;
   vec3 direction;
   float cutOff;

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
uniform Light    light;

void main()
{
   vec3 lightDir   = normalize(light.position - FragPos);

   // Cosine of the angle between the direction of the spotlight and the direction of the light ray to the fragment
   float theta     = dot(lightDir, normalize(-light.direction));

   if (theta > light.cutOff)
   {
      // Ambient
      vec3 ambient      = light.ambient * texture(material.diffuse, TexCoords).rgb;

      // Diffuse
      vec3 norm         = normalize(Normal);
      float diff        = max(dot(norm, lightDir), 0.0);
      vec3 diffuse      = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;

      // Specular
      vec3 viewDir      = normalize(viewPos - FragPos);
      vec3 reflectDir   = reflect(-lightDir, norm);
      float spec        = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
      vec3 specular     = light.specular * spec * texture(material.specular, TexCoords).rgb;

      // Attenuation
      float distance    = length(light.position - FragPos);
      float attenuation = 1.0 / (light.constant + (light.linear * distance) + (light.quadratic * distance * distance));

      // Do not attenuate the ambient light. Otherwise, the objects inside the spotlight will be darker than
      // the objects outside of it at large distances.
      diffuse          *= attenuation;
      specular         *= attenuation;

      vec3 result       = ambient + diffuse + specular;
      FragColor         = vec4(result, 1.0);
   }
   else
   {
      vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
      FragColor    = vec4(ambient, 1.0);
   }
}
