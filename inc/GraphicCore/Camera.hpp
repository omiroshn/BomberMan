#ifndef BOMBERMAN_CAMERA_HPP
#define BOMBERMAN_CAMERA_HPP

#include "GL/glew.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

enum CameraDirection
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Default camera values
const float SPEED       =  2.5f;
const float SENSITIVITY =  0.1f;
const float ZOOM        =  45.0f;


// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:

    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = 0.f, float pitch = 0.f);
    glm::mat4 getViewMatrix();
    void movaCamera(CameraDirection dir, float deltaTime);
    void processMouseMovement(float xoffset, float yoffset);
    void processMouseScroll(float zoomFactor);

    float zoom() const;
private:
    void updateCameraVectors();

private:
    glm::vec3 mPosition;
    glm::vec3 mFront;
    glm::vec3 mUp;
    glm::vec3 mRight;
    glm::vec3 mWorldUp;

    float mYaw;
    float mPitch;

    float mMovementSpeed;
    float mMouseSensitivity;
    float mZoom;
};
#endif //BOMBERMAN_CAMERA_HPP
