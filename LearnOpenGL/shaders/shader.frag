#version 330 core

out vec4 color;

in VS_OUT
{
    vec3 color;
    vec2 texCoord;
} fs_in;

uniform sampler2D tex0;
uniform sampler2D tex1;
uniform float uCross;

void main()
{
    /*
     * You probably noticed that the texture is flipped upside-down! This happens because OpenGL expects 
     * the 0.0 coordinate on the y-axis to be on the bottom side of the image, but images usually have 0.0 
     * at the top of the y-axis. This is a small "hack" to fix the issue (for now).
     */
    vec2 flipY = vec2(fs_in.texCoord.x, 1.0 - fs_in.texCoord.y);
    color = mix(texture(tex0, flipY), texture(tex1, flipY), uCross);
}