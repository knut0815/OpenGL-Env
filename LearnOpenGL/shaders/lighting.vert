#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

out VS_OUT
{
    vec3 color;
    vec2 texCoord;
    vec3 normal;
    vec3 worldPos;
} vs_out;

uniform mat4 uModelViewProjection;
uniform mat4 uModel;
uniform vec3 uViewPos;

void main()
{
    /* 
     * Inversing matrices is a costly operation even for shaders so wherever possible, 
     * try to avoid doing inverse operations in shaders since they have to be done on 
     * each vertex of your scene. For learning purposes this is fine, but for an efficient 
     * application you'll likely want to calculate the normal matrix on the CPU and send 
     * it to the shaders via a uniform before drawing (just like the model matrix).
     */
    mat3 normalMatrix = mat3(transpose(inverse(uModel)));
    vs_out.normal = normalMatrix * normal;
    vs_out.texCoord = texCoord;
    vs_out.worldPos = vec3(uModel * vec4(position, 1.0f));
    
    gl_Position = uModelViewProjection * vec4(position, 1.0);
}