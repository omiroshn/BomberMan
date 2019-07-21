#ifndef BOMBERMAN_CAMERA_HPP
#define BOMBERMAN_CAMERA_HPP

#include "GL/glew.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

enum class CameraDirection
{
    FORWARD = 0,
    BACKWARD,
    LEFT,
    RIGHT,
    UPWARD,
    DOWNWARD
};

// Default camera values
const float SPEED       =  0.1f;
const float SENSITIVITY =  0.3f;
const float ZOOM        =  45.0f;

class Entity;
// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = 0.f, float pitch = 0.f);
    glm::mat4 getViewMatrix();
    void moveCamera(CameraDirection dir, float deltaTime);
    void processMouseMovement(float xoffset, float yoffset);
    void processMouseScroll(float zoomFactor);
    void addShake(float amount);

    float zoom() const;
    glm::vec3 position() const;
    void followEntity(Entity &, float d, float deltaTime);
	Camera(Camera const&) = delete;
    Camera& operator=(Camera const&) = delete;
    ~Camera();
private:
    void updateCameraVectors();
    void applyTransform();
private:
    glm::vec3 mPosition;
    glm::vec3 mFront;
    glm::vec3 mUp;
    glm::vec3 mRight;
    glm::vec3 mWorldUp;

    glm::mat4 mViewMatrix;
    float mYaw;
    float mPitch;
    float mShakeAmount{.0f};

    float mMovementSpeed;
    float mMouseSensitivity;
    float mZoom;
};
#endif //BOMBERMAN_CAMERA_HPP
