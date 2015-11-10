#version 330 core

out vec4 color;

//=================================================================== From vertex shader
in VS_OUT
{
    vec3 color;
    vec2 texCoord;
    vec3 normal;
    vec3 worldPos;
} fs_in;

//=================================================================== Material properties
struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

uniform Material material;
uniform vec3 uViewPos;

//=================================================================== Directional light(s)
struct DirLight
{
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform DirLight dirLight;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    
    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    
    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    
    // Combine results
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, fs_in.texCoord));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, fs_in.texCoord));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, fs_in.texCoord));
    
    return (ambient + diffuse + specular);
}

//=================================================================== Point light(s)
struct PointLight
{
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

const int NR_POINT_LIGHTS = 4;
uniform PointLight pointLights[NR_POINT_LIGHTS];

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    
    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    
    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    
    // Attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    
    // Combine results
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, fs_in.texCoord));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, fs_in.texCoord));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, fs_in.texCoord));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    
    return (ambient + diffuse + specular);
}

//=================================================================== Spotlight(s)
struct SpotLight
{
    vec3 position;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    
    float constant;
    float linear;
    float quadratic;
    
    vec3 direction;
    float cutoff;
    float outerCutoff;
};

uniform SpotLight spotLight;

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    // Ambient shading
    vec3 ambient            = light.ambient * vec3(texture(material.diffuse, fs_in.texCoord));
    
    // Diffuse shading
    vec3 lightDir           = normalize(light.position - fragPos);
    float diffuseStrength   = max(dot(normal, lightDir), 0.0);
    vec3 diffuse            = light.diffuse * diffuseStrength * vec3(texture(material.diffuse, fs_in.texCoord));
    
    // Specular shading
    vec3 reflectDir         = reflect(-lightDir, normal);
    float specularStrength  = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular           = light.specular * specularStrength * vec3(texture(material.specular, fs_in.texCoord));
    
    // Inner and outer cone
    float theta             = dot(lightDir, normalize(-light.direction));
    float epsilon           = light.cutoff - light.outerCutoff;
    float intensity         = clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0);
    
    // Attenuation
    float distance      = length(light.position - fragPos);
    float attenuation   = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    
    // Combine results
    ambient     *= attenuation * intensity;
    diffuse     *= attenuation * intensity;
    specular    *= attenuation * intensity;
    
    return (ambient + specular + diffuse);
}

//=================================================================== Main
void main()
{
    vec3 norm = normalize(fs_in.normal);
    vec3 viewDir = normalize(uViewPos - fs_in.worldPos);
    
    // Directional lighting
    vec3 result = CalcDirLight(dirLight, norm, viewDir);
    
    // Point light(s)
    for (int i = 0; i < NR_POINT_LIGHTS; ++i)
    {
        result += CalcPointLight(pointLights[i], norm, fs_in.worldPos, viewDir);
    }
    
    // Spotlight
    result += CalcSpotLight(spotLight, norm, fs_in.worldPos, viewDir);
    
    color = vec4(result, 1.0);
    
}