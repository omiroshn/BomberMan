//
// Created by Vadim on 2019-02-13.
//

#include "GraphicCore/Camera.hpp"
#include "Entity/Entity.h"
#include <iostream>
#include "imgui.h"
Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) :
mFront(glm::vec3(0.0f, 0.0f, -1.0f)), mMovementSpeed(SPEED), mMouseSensitivity(SENSITIVITY), mZoom(ZOOM),
mPosition(position), mWorldUp(up), mYaw(yaw), mPitch(pitch)
{
    updateCameraVectors();
}

// Returns the view matrix calculated using Euler Angles and the LookAt Matrix
glm::mat4 Camera::getViewMatrix()
{
    return mViewMatrix;
}

void Camera::followEntity(Entity &aTarget, float d)
{
	static float zoom = 1;
	ImGui::SliderFloat("Zoom", &zoom, 0.2f, 4.f);
	d *= zoom;
    mPosition.x = aTarget.getPosition().x;
	mPosition.y = d * 1.4f;
    mPosition.z = aTarget.getPosition().y + d;
//    mViewMatrix = glm::lookAt(mPosition, aTarget.getPosition3D(), mUp);
}
void Camera::applyTransform()
{
    mViewMatrix = glm::lookAt(mPosition, mPosition + mFront, mUp);
}

void Camera::movaCamera(CameraDirection dir, float deltaTime)
{
    float velocity = mMovementSpeed * deltaTime;
    if (dir == CameraDirection::FORWARD)
    {
        mPosition += mFront * velocity;
    }
    else if (dir == CameraDirection::BACKWARD)
    {
        mPosition -= mFront * velocity;
    }
    else if (dir == CameraDirection::LEFT)
    {
        mPosition -= mRight * velocity;
    }
    else if (dir == CameraDirection::RIGHT)
    {
        mPosition += mRight * velocity;
    }
    else if (dir == CameraDirection::UPWARD)
    {
        mPosition += mUp * velocity;
    }
    else if (dir == CameraDirection::DOWNWARD)
    {
        mPosition -= mUp * velocity;
    }
    applyTransform();
}

void Camera::processMouseMovement(float xoffset, float yoffset)
{
    mYaw   += xoffset * mMouseSensitivity;
    mPitch += yoffset * mMouseSensitivity;
    updateCameraVectors();
    applyTransform();
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

glm::vec3 Camera::position() const
{
    return mPosition;
}