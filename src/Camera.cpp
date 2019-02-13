//
// Created by Vadim on 2019-02-13.
//

#include "Camera.hpp"
#include <iostream>

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) :
mFront(glm::vec3(0.0f, 0.0f, -1.0f)), mMovementSpeed(SPEED), mMouseSensitivity(SENSITIVITY), mZoom(ZOOM),
mPosition(position), mWorldUp(up), mYaw(yaw), mPitch(pitch)
{
    updateCameraVectors();
}

// Returns the view matrix calculated using Euler Angles and the LookAt Matrix
glm::mat4 Camera::getViewMatrix()
{
    return glm::lookAt(mPosition, mPosition + mFront, mUp);
}

void Camera::movaCamera(CameraDirection dir, float deltaTime)
{
    float velocity = mMovementSpeed * deltaTime;
    if (dir == FORWARD)
    {
        std::cout << "FORWARD" << std::endl;
        mPosition += mFront * velocity;
    }
    else if (dir == BACKWARD)
    {
        std::cout << "BACKWARD" << std::endl;
        mPosition -= mFront * velocity;
    }
    else if (dir == LEFT)
    {
        std::cout << "LEFT" << std::endl;
        mPosition -= mRight * velocity;
    }
    else if (dir == RIGHT)
    {
        std::cout << "RIGHT" << std::endl;
        mPosition += mRight * velocity;
    }
}

void Camera::processMouseMovement(float xoffset, float yoffset)
{
    mYaw   += xoffset * mMouseSensitivity;
    mPitch += yoffset * mMouseSensitivity;

    updateCameraVectors();
}

void Camera::processMouseScroll(float zoomFactor)
{
    if (mZoom >= 1.0f && mZoom <= 45.0f)
        mZoom -= zoomFactor;
    if (mZoom <= 1.0f)
        mZoom = 1.0f;
    if (mZoom >= 45.0f)
        mZoom = 45.0f;
}

void Camera::updateCameraVectors()
{
    // Calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(mYaw)) * cos(glm::radians(mPitch));
    front.y = sin(glm::radians(mPitch));
    front.z = sin(glm::radians(mYaw)) * cos(glm::radians(mPitch));
    mFront = glm::normalize(front);
    // Also re-calculate the Right and Up vector
    mRight = glm::normalize(glm::cross(mFront, mWorldUp));
    mUp    = glm::normalize(glm::cross(mRight, mFront));
}

float Camera::zoom() const
{
    return mZoom;
}