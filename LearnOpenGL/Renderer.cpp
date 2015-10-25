#include "Renderer.h"

#define STRINGIFY(x) #x

static const std::string GLSL_VERSION = "#version 330 core\n";

static const std::string DEFAULT_VERTEX_SHADER = GLSL_VERSION +
STRINGIFY (
    layout (location = 0) in vec3 position;
    void main()
    {
        gl_Position = position;
    }
);

static const std::string DEFAULT_FRAGMENT_SHADER = GLSL_VERSION +
STRINGIFY (
    out vec4 outputColor;
    void main()
    {
        outputColor = vec4(1.0);
    }
);

Renderer::Renderer()
{
    
}

void Renderer::setupDefaultGraphics()
{
    bUsingDefaultShader = true;
    defaultShader.setupProgramFromSource(DEFAULT_VERTEX_SHADER, DEFAULT_FRAGMENT_SHADER);
}

void Renderer::setColor(float r, float g, float b, float a)
{
    currentDrawColor.set(r, g, b, a);
}

void Renderer::setColor(Color col)
{
    currentDrawColor = col;
}

void Renderer::clear(float r, float g, float b, float a) const
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
}

void Renderer::clear(Color col) const
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(col.getRed() / 255.0f, col.getGreen() / 255.0f, col.getBlue() / 255.0f, col.getAlpha() / 255.0f);
}