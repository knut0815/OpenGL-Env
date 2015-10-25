#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 texCoord;

out VS_OUT
{
    vec3 color;
    vec2 texCoord;
} vs_out;

uniform mat4 uModelViewProjection;

void main()
{
    vs_out.color = color;
    vs_out.texCoord = texCoord;
    gl_Position = uModelViewProjection * vec4(position, 1.0);
}