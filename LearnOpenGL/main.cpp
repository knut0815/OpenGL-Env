#include <math.h>
#include <iostream>

/* 
 * Here, we choose to use the static version of the GLEW library.
 * For more information on static vs. dynamic linkage, refer to the
 * end of the section titled "Creating a Window" at learnopengl.com.
 * Also, note that the order here matters: we need to include GLEW
 * before GLFW. The include file for GLEW contains the correct
 * OpenGL header includes (like GL/gl.h), so including GLEW before other
 * header files that require OpenGL does the trick. 
 */
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW and OpenGL
#include <GLFW/glfw3.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Custom headers
#include "GlslProgram.h"
#include "Image.h"
#include "Camera.h"

GLFWwindow *window;
const GLuint WINDOW_WIDTH = 800;
const GLuint WINDOW_HEIGHT = 600;
bool keys[1024];
GLfloat deltaTime = 0.0f;                                           // Time between current frame and last frame
GLfloat lastFrame = 0.0f;                                           // Time of last frame
GLfloat lastX = WINDOW_WIDTH / 2;
GLfloat lastY = WINDOW_HEIGHT / 2;
bool firstMouse = true;

Camera cam;

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if(firstMouse)                                                  // This ensures that the first time we move the mouse, we don't see a huge jump
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    
    GLfloat xoffset = xpos - lastX;
    GLfloat yoffset = lastY - ypos;                                 // Reversed since y-coordinates range from bottom to top
    lastX = xpos;
    lastY = ypos;
    
    cam.processMouse(xoffset, yoffset);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    /*
     * The trick is to only keep track of what keys are pressed/released in the callback function. 
     * In the game loop we then read these values to check what keys are active and react accordingly.
     * So we're basically storing state information about what keys are pressed/released and react 
     * upon that state in the game loop.
     */
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        /*
         * When a user presses the escape key, we set the WindowShouldClose property to true,
         * closing the application.
         */
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS) keys[key] = true;
        else if (action == GLFW_RELEASE) keys[key] = false;
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    cam.processScroll(yoffset);
}

void calculateCameraMovement()
{
    if (keys[GLFW_KEY_W])
        cam.processKeyboard(FORWARD, deltaTime);
    if (keys[GLFW_KEY_S])
        cam.processKeyboard(BACKWARD, deltaTime);
    if (keys[GLFW_KEY_A])
        cam.processKeyboard(LEFT, deltaTime);
     if (keys[GLFW_KEY_D])
        cam.processKeyboard(RIGHT, deltaTime);
}

int main(int argc, const char * argv[])
{
    glfwInit();                                                     // Instantiate GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);                  // Tell GLFW that we want to use version 3.3 of OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // Use the core profile
    //glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);                       // Don't let the user resize the window
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_SAMPLES, 4);
    
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "LearnOpenGL", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window." << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    
    glfwSetKeyCallback(window, key_callback);                       // Register our callbacks
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    glewExperimental = GL_TRUE;                                     // We want to use more modern techniques for managing OpenGL
    if (glewInit() != GLEW_OK)                                      // Initialize GLEW, which manages function pointers for OpenGL

    {
        std::cout << "Failed to initialize GLEW." << std::endl;
        return -1;
    }
    
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);                  // Tell OpenGL the size of the rendering window
    glEnable(GL_DEPTH_TEST);
    
    // Set up vertex data (and buffer(s)) and attribute pointers
    GLfloat vertices[] = {
        // Positions          // Normals           // Texture Coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
        
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
        
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
        
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };
    
    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };
    
    glm::vec3 pointLightPositions[] = {
        glm::vec3( 0.7f,  0.2f,  2.0f),
        glm::vec3( 2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3( 0.0f,  0.0f, -3.0f)
    };
    
    GLuint VAO, cubeVBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &cubeVBO);
    
    /*
     * A vertex array object (VAO) can be bound just like a vertex buffer object and any subsequent
     * vertex attribute calls from that point on will be stored inside the VAO. This has the advantage
     * that when configuring vertex attribute pointers you only have to make those calls once and
     * whenever we want to draw the object, we can just bind the corresponding VAO. This makes switching
     * between different vertex data and attribute configurations as easy as binding a different VAO.
     * All the state we just set is stored inside the VAO.
     * In particular, a VAO stores:
     * 1) Calls to glEnableVertexAttribArray or glDisableVertexAttribArray
     * 2) Vertex attribute configurations via glVertexAttribPointer
     * 3) Vertex buffer objects associated with vertex attributes by calls to glVertexAttribPointer
     */
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);                         // Any future buffer calls we make will be used to configure VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    /*
     * The parameters of glVertexAttribPointer are as follows:
     * 1) The vertex attribute that we want to configure (corresponds to the layout qualifier in the shader)
     * 2) The size of the vertex attribute
     * 3) The type of the data
     * 4) Do we want the data normalized?
     * 5) The stride (we could just use 0 since our values are tightly packed)
     * 6) The offset
     * The data is pulled from the buffer that is currently bound to the GL_ARRAY_BUFFER targer (i.e. VBO)
     */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);                               // Unbind the VBO (good practice); note that we must leave the EBO bound
    glBindVertexArray(0);
    
    
    GLuint lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);                         // We only need to bind to the VBO, the container's VBO's data already contains the correct data.

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);   // Set the vertex attributes (only position data for our lamp)
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
    
    
    GlslProgram cubeProgram;
    cubeProgram.setupProgramFromFile("shaders/lighting.vert", "shaders/multilight.frag");
    
    GlslProgram lightProgram;
    lightProgram.setupProgramFromFile("shaders/source.vert", "shaders/source.frag");
    
    Image tex0;
    tex0.loadImage("assets/diffuse_map.png", 500, 500);
    
    Image tex1;
    tex1.loadImage("assets/specular_map.png", 500, 500);
    
    /*
     * Everything that follows is our "game" or "rendering" loop. This will keep executing
     * until GLFW has been instructed to close. The glfwPollEvents function checks if any
     * events are triggered and calls the corresponding functions. The glfwSwapBuffers function
     * does exactly what it says. This technique is known as "double buffering." The front buffer 
     * contains the final output image that is shown at the screen, while all the rendering 
     * commands draw to the back buffer. As soon as all the rendering commands are finished we swap 
     * the back buffer to the front buffer so the image is instantly displayed to the user, removing 
     * any would-be drawing artifacts.
     */
    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        
        /* 
         * Currently we used a constant value for movement speed when walking around. 
         * In theory this seems fine, but in practice people have different processing 
         * powers and the result of that is that some people are able to draw much more 
         * frames than others each second. Whenever a user draws more frames than another 
         * user he also calls calculateCameraMovement() more often. The result is that some 
         * people move really fast and some really slow depending on their setup. When 
         * shipping your application you want to make sure it runs the same on all kinds 
         * of hardware.
         */
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        calculateCameraMovement();
        
        // ===============================
        // Rendering starts here
        // ===============================
        
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);                       // A state-setting function that sets the clear color
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);         // A state-using function that clears the active buffer
        
        
        //=================================================================== Cube program begins
        cubeProgram.begin();
        
        glBindVertexArray(VAO);
        
        /*
         * The parameters of glm::perspective are as follows
         * 1) The FOV (in radians)
         * 2) The aspect ratio
         * 3 / 4) The near and far clipping planes
         */
        glm::mat4 model;
        glm::mat4 uModelViewProjection;
        glm::mat4 projection = glm::perspective(glm::radians(cam.getFOV()), WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
        cubeProgram.setUniform3f("uViewPos", cam.getPositionVector().x, cam.getPositionVector().y, cam.getPositionVector().z);
        ///
        /*
         * The default texture unit for a texture is 0, which is the default active texture unit so we
         * did not need to assign a location to this texture before binding it. If, however, we want to bind
         * multiple textures simultaneously, we will need to manually assign texture units.
         * To use the second texture (and the first texture) we have to change the rendering procedure
         * a bit by binding both textures to the corresponding texture unit and specifying which uniform
         * sampler corresponds to which texture unit.
         */
        cubeProgram.setUniformSampler2D("material.diffuse", tex0, 0);
        cubeProgram.setUniformSampler2D("material.specular", tex1, 1);
        cubeProgram.setUniform1f("material.shininess", 32.0f);
        
        // Directional light
        cubeProgram.setUniform3f("dirLight.direction", -0.2f, -1.0f, -0.3f);
        cubeProgram.setUniform3f("dirLight.ambient", 0.05f, 0.05f, 0.05f);
        cubeProgram.setUniform3f("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
        cubeProgram.setUniform3f("dirLight.specular", 0.5f, 0.5f, 0.5f);
        
        // Point light 1
        cubeProgram.setUniform3f("pointLights[0].position", pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
        cubeProgram.setUniform3f("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
        cubeProgram.setUniform3f("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
        cubeProgram.setUniform3f("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
        cubeProgram.setUniform1f("pointLights[0].constant", 1.0f);
        cubeProgram.setUniform1f("pointLights[0].linear", 0.09);
        cubeProgram.setUniform1f("pointLights[0].quadratic", 0.032);
        
        // Point light 2
        cubeProgram.setUniform3f("pointLights[1].position", pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
        cubeProgram.setUniform3f("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
        cubeProgram.setUniform3f("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
        cubeProgram.setUniform3f("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
        cubeProgram.setUniform1f("pointLights[1].constant", 1.0f);
        cubeProgram.setUniform1f("pointLights[1].linear", 0.09);
        cubeProgram.setUniform1f("pointLights[1].quadratic", 0.032);
        
        // Point light 3
        cubeProgram.setUniform3f("pointLights[2].position", pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
        cubeProgram.setUniform3f("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
        cubeProgram.setUniform3f("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
        cubeProgram.setUniform3f("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
        cubeProgram.setUniform1f("pointLights[2].constant", 1.0f);
        cubeProgram.setUniform1f("pointLights[2].linear", 0.09);
        cubeProgram.setUniform1f("pointLights[2].quadratic", 0.032);
        
        // Point light 4
        cubeProgram.setUniform3f("pointLights[3].position", pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
        cubeProgram.setUniform3f("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
        cubeProgram.setUniform3f("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
        cubeProgram.setUniform3f("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
        cubeProgram.setUniform1f("pointLights[3].constant", 1.0f);
        cubeProgram.setUniform1f("pointLights[3].linear", 0.09);
        cubeProgram.setUniform1f("pointLights[3].quadratic", 0.032);
        
        // Spotlight
        cubeProgram.setUniform3f("spotLight.position", cam.getPositionVector().x, cam.getPositionVector().y, cam.getPositionVector().z);
        cubeProgram.setUniform3f("spotLight.direction", cam.getFrontVector().x, cam.getFrontVector().y, cam.getFrontVector().z);
        cubeProgram.setUniform3f("spotLight.ambient", 0.0f, 0.0f, 0.0f);
        cubeProgram.setUniform3f("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
        cubeProgram.setUniform3f("spotLight.specular", 1.0f, 1.0f, 1.0f);
        cubeProgram.setUniform1f("spotLight.constant", 1.0f);
        cubeProgram.setUniform1f("spotLight.linear", 0.09);
        cubeProgram.setUniform1f("spotLight.quadratic", 0.032);
        cubeProgram.setUniform1f("spotLight.cutoff", glm::cos(glm::radians(12.5f)));
        cubeProgram.setUniform1f("spotLight.outerCutoff", glm::cos(glm::radians(15.0f)));
        
        for(GLuint i = 0; i < 10; ++i)
        {
            GLfloat angle = 20.0f * i;
            model = glm::mat4();
            model = glm::translate(model, cubePositions[i]);
            model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
            uModelViewProjection = projection * cam.getViewMatrix() * model;
            cubeProgram.setUniform4x4Matrix("uModel", model);
            cubeProgram.setUniform4x4Matrix("uModelViewProjection", uModelViewProjection);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        
        tex0.unbind();
        tex1.unbind();
        
        cubeProgram.end();
        //=================================================================== Cube program ends
    
        
        //=================================================================== Light program begins
        lightProgram.begin();
        
        glBindVertexArray(lightVAO);
        
        for (GLuint i = 0; i < 4; ++i)
        {
            model = glm::mat4();
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.2f));
            uModelViewProjection = projection * cam.getViewMatrix() * model;
            lightProgram.setUniform4x4Matrix("uModelViewProjection", uModelViewProjection);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        
        lightProgram.end();
        //=================================================================== Light program ends
        
        
        glBindVertexArray(0);
        

        // ===============================
        // Rendering ends here
        // ===============================
        
        glfwSwapBuffers(window);
    }
    
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &cubeVBO);
    
    glfwTerminate();                                                
    std::cout << "Terminating the application." << std::endl;
    return 0;
}
