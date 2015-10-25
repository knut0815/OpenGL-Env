#ifndef __LearnOpenGL__glRenderer__
#define __LearnOpenGL__glRenderer__

#include <string>
#include <GL/glew.h>
#include "GlslProgram.h"
#include "Color.h"

class Renderer
{

public:
    
    Renderer();
    void setupDefaultGraphics();
    void setColor(float r, float g, float b, float a);
    void setColor(Color col);
    void clear(float r, float g, float b, float a) const;
    void clear(Color col) const;
    
private:
    
    Color currentDrawColor;
    GlslProgram defaultShader;
    bool bUsingDefaultShader;
    
    // Uniforms
    const std::string COLOR_UNIFORM = "uCurrentColor";
    
};
#endif
