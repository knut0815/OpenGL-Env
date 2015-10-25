#ifndef __LearnOpenGL__basicApp__
#define __LearnOpenGL__basicApp__

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class BasicApp
{
    
public:
    
    BasicApp(int width = 800, int height = 600);
    void setup();
    void update();
    void draw();
    
    GLint getMaxNumVertAttribs();
    inline int getWidth() const { return viewportWidth; }
    inline int getHeight() const { return viewportHeight; }
    inline GLFWwindow * getViewport() const { return viewport; }
    
private:
    
    GLFWwindow *viewport;
    int viewportWidth;
    int viewportHeight;
    
    void setupWindow();
    void registerCallbacks();
    
};

#endif
