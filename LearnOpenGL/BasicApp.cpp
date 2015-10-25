#include "BasicApp.h"

// ===============================
// Public member functions
// ===============================

BasicApp::BasicApp(int width, int height) : viewportWidth(width), viewportHeight(height)
{
    glfwInit();                                                     // Instantiate GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);                  // Tell GLFW that we want to use version 3.3 of OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // Use the core profile
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);                       // Don't let the user resize the window
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_SAMPLES, 4);
    
    setupWindow();
}

GLint BasicApp::getMaxNumVertAttribs()
{
    GLint nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    return nrAttributes;
}

// ===============================
// Private member functions
// ===============================

void BasicApp::setupWindow()
{
    viewport = glfwCreateWindow(viewportWidth, viewportHeight, "Basic App", nullptr, nullptr);
    if (viewport == nullptr)
    {
        std::cout << "Failed to create GLFW window." << std::endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(viewport);

    glfwSetInputMode(viewport, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    glewExperimental = GL_TRUE;                                     // We want to use more modern techniques for managing OpenGL
    if (glewInit() != GLEW_OK)                                      // Initialize GLEW, which manages function pointers for OpenGL
        
    {
        std::cout << "Failed to initialize GLEW." << std::endl;
        return;
    }
    
    glViewport(0, 0, viewportWidth, viewportHeight);                // Tell OpenGL the size of the rendering window
}

void BasicApp::registerCallbacks()
{
//    glfwSetKeyCallback(viewport, keyPressed());                       
//    glfwSetCursorPosCallback(viewport, mouse_callback);
//    glfwSetScrollCallback(window, scroll_callback);
}