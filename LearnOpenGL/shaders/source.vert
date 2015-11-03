#version 330 core

layout (location = 0) in vec3 position;

uniform mat4 uModelViewProjection;

void main()
{
    gl_Position = uModelViewProjection * vec4(position, 1.0);
}