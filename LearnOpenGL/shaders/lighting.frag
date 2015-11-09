#version 330 core

out vec4 color;

in VS_OUT
{
    vec3 color;
    vec2 texCoord;
    vec3 normal;
    vec3 worldPos;
} fs_in;

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light
{
    vec3 position;
    
    // Material properties
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    
    // Attenuation constants
    float constant;
    float linear;
    float quadratic;
    
    // Spotlight control
    vec3 direction;
    float cutoff;
    float outerCutoff;
};

uniform Material material;
uniform Light light;
uniform vec3 uViewPos;

void main()
{
    // Calculate the ambient strength, which we always use
    vec3 ambient            = light.ambient * vec3(texture(material.diffuse, fs_in.texCoord));
    
    // Calculate the diffuse strength
    vec3 norm               = normalize(fs_in.normal);
    vec3 lightDir           = normalize(light.position - fs_in.worldPos);
    float diffuseStrength   = max(dot(norm, lightDir), 0.0);
    vec3 diffuse            = light.diffuse * diffuseStrength * vec3(texture(material.diffuse, fs_in.texCoord));
    
    // Calculate the specular strength
    vec3 viewDir            = normalize(uViewPos - fs_in.worldPos);
    vec3 reflectDir         = reflect(-lightDir, norm);
    float specularStrength  = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular           = light.specular * specularStrength * vec3(texture(material.specular, fs_in.texCoord));
    
    // Calculate the effect of our spotlight (with soft edges)
    float theta         = dot(lightDir, normalize(-light.direction));
    float epsilon       = light.cutoff - light.outerCutoff;
    float intensity     = clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0);
    diffuse             *= intensity;
    specular            *= intensity;
    
    // Calculate the attenuation factors for this fragment relative to the light position
    float distance      = length(light.position - fs_in.worldPos);
    float attenuation   = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    ambient     *= attenuation;
    diffuse     *= attenuation;
    specular    *= attenuation;
    
    color = vec4(ambient + diffuse + specular, 1.0);

}