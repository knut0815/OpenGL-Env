#ifndef __LearnOpenGL__camera__
#define __LearnOpenGL__camera__

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

enum Direction
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

static const GLfloat SPEED = 3.0f;
static const GLfloat SENSITIVITY = 0.25f;

class Camera
{
    
public:
    
    Camera();
    Camera(glm::vec3 &position, glm::vec3 &target, glm::vec3 &up);
    glm::vec3 getFrontVector() const { return camFront; }
    glm::vec3 getPositionVector() const { return camPosition; }
    glm::vec3 getTargetVector() const { return camTarget; }
    glm::vec3 getUpVector() const { return camUp; }
    glm::mat4 getViewMatrix() const { return glm::lookAt(camPosition, camPosition + camFront, camUp); }
    GLfloat getYaw() const { return camYaw; }
    GLfloat getPitch() const { return camPitch; }
    GLfloat getFOV() const { return camFOV; }
    void processKeyboard(Direction dir, GLfloat deltaTime);
    void processMouse(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true);
    void processScroll(GLfloat yoffset);
    
private:

    glm::vec3 camRight;
    glm::vec3 camFront;
    glm::vec3 camPosition;
    glm::vec3 camTarget;
    glm::vec3 camUp;
    glm::mat4 camView;
    GLfloat camYaw;
    GLfloat camPitch;
    GLfloat camFOV;
    
    void init();
    void updateVectors();
    
};
#endif
