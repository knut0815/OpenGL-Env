#include "Camera.h"

// ===============================
// Public member functions
// ===============================

Camera::Camera()
{
    camFront = glm::vec3(0.0f, 0.0f, -1.0f);
    camPosition = glm::vec3(0.0f, 0.0f, 3.0f);
    camTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    camUp = glm::vec3(0.0f, 1.0f, 0.0f);
    
    init();
    updateVectors();
}

Camera::Camera(glm::vec3 &position, glm::vec3 &target, glm::vec3 &up) :
            camPosition(position), camTarget(target), camUp(up)
{
    init();
    updateVectors();
}

void Camera::processKeyboard(Direction dir, GLfloat deltaTime)
{
    GLfloat velocity = SPEED * deltaTime;
    switch (dir)
    {
        case FORWARD:
            camPosition += camFront * velocity;
            break;
        case BACKWARD:
            camPosition -= camFront * velocity;
            break;
        case LEFT:
            camPosition -= camRight * velocity;
            break;
        case RIGHT:
            camPosition += camRight * velocity;
            break;
        default:
            break;
    }
}

void Camera::processMouse(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch)
{
    xoffset *= SENSITIVITY;
    yoffset *= SENSITIVITY;
    
    camYaw   += xoffset;
    camPitch += yoffset;
    
    // Make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
        if (camPitch > 89.0f) camPitch = 89.0f;
        if (camPitch < -89.0f) camPitch = -89.0f;
    }
    updateVectors();
}

void Camera::processScroll(GLfloat yoffset)
{
    if (camFOV >= 1.0f && camFOV <= 45.0f)
        camFOV -= yoffset;
    if (camFOV <= 1.0f)
        camFOV = 1.0f;
    if (camFOV >= 45.0f)
        camFOV = 45.0f;
}

// ===============================
// Private member functions
// ===============================

void Camera::init()
{
    camYaw = -90.0f;
    camPitch = 0.0f;
    camFOV = 45.0f;
}

void Camera::updateVectors()
{
    glm::vec3 front;
    front.x = cos(glm::radians(camPitch)) * cos(glm::radians(camYaw));
    front.y = sin(glm::radians(camPitch));
    front.z = cos(glm::radians(camPitch)) * sin(glm::radians(camYaw));
    camFront = glm::normalize(front);
    
    camRight = glm::normalize(glm::cross(camFront, glm::vec3(0.0f, 1.0f, 0.0f)));
    camUp = glm::normalize(glm::cross(camRight, camFront));
}