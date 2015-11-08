#version 330 core

out vec4 color;

in VS_OUT
{
    vec3 color;
    vec2 texCoord;
    vec3 normal;
    vec3 worldPos;
    vec3 worldCamPos;
} fs_in;

uniform vec3 uObjectColor;
uniform vec3 uLightColor;
uniform vec3 uLightPos;
uniform vec3 uViewPos;
uniform sampler2D tex0;
uniform sampler2D tex1;
uniform float uCross;

void main()
{
#ifdef USING_TEXTURES
    /*
     * You probably noticed that the texture is flipped upside-down! This happens because OpenGL expects
     * the 0.0 coordinate on the y-axis to be on the bottom side of the image, but images usually have 0.0
     * at the top of the y-axis. This is a small "hack" to fix the issue (for now).
     */
    vec2 flipY = vec2(fs_in.texCoord.x, 1.0 - fs_in.texCoord.y);
    color = mix(texture(tex0, flipY), texture(tex1, flipY), uCross);
#else
    // The ambient term is just a small fraction of our light's color multiplied by our object's color
    float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * uLightColor;
    
    // The diffuse term is calculated by multiplying the surface normal by a direction vector pointing from the light source to the current fragment
    vec3 n = normalize(fs_in.normal);
    vec3 dir = normalize(uLightPos - fs_in.worldPos);
    float diffuseStrength = max(dot(n, dir), 0.0f); // If the angle is greater than 90 degrees, we don't want to return a negative value
    vec3 diffuse = diffuseStrength * uLightColor;
    
    float specularStrength = 0.5f;
    vec3 viewDir = normalize(uViewPos - fs_in.worldPos);
    vec3 reflectDir = reflect(-dir, n);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * uLightColor;
    
    vec3 result = (ambient + diffuse + specular) * uObjectColor;
    color = vec4(result, 1.0f);
#endif
    
}