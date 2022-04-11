
#include <iostream>
#include "Core/Maths.hpp"
#include "Core/Physics.hpp"
#include "Core/Camera.hpp"
#include "Core/Constants.hpp"

LowRenderer::Camera::Camera(int width, int height)
{
    aspect = (float)width / (float)height;
}

void LowRenderer::Camera::fpsMove(float deltaTime, const CameraInputs& inputs)
{
    yaw -= inputs.deltaX * MOUSE_SENSITIVITY;
    pitch += inputs.deltaY * MOUSE_SENSITIVITY;

    position = toFollow->position + offsetFromTarget;
    
    toFollow->rotation.y = yaw;
}

void LowRenderer::Camera::freeMove(float deltaTime, const CameraInputs& inputs)
{
    yaw -= inputs.deltaX * MOUSE_SENSITIVITY;
    pitch += inputs.deltaY * MOUSE_SENSITIVITY;

    float forward_movement = 0.f;
    float horizontal_movement = 0.f;
    float vertical_movement = 0.f;

    if (inputs.moveForward)
        forward_movement -= CAMERA_SPEED * deltaTime;
    if (inputs.moveBackward)
        forward_movement += CAMERA_SPEED * deltaTime;
    if (inputs.moveLeft)
        horizontal_movement -= CAMERA_SPEED * deltaTime;
    if (inputs.moveRight)
        horizontal_movement += CAMERA_SPEED * deltaTime;
    if (inputs.moveUpwards)
        vertical_movement += CAMERA_SPEED * deltaTime;
    if (inputs.moveDownwards)
        vertical_movement -= CAMERA_SPEED * deltaTime;

    position.x += (sinf(yaw) * forward_movement) + (cosf(yaw) * horizontal_movement);
    position.z += (cosf(yaw) * forward_movement) + (-sinf(yaw) * horizontal_movement);

    position.y += vertical_movement;
}

void LowRenderer::Camera::followMove()
{
    if (toFollow == nullptr) 
    {
        std::cout << "a";
        return;
    }

    position = position + (((toFollow->position + offsetFromTarget) - position.xyz) * CAMERA_LERP);
}

void LowRenderer::Camera::update(float deltaTime, const CameraInputs& inputs)
{
    switch (state)
    {
    case LowRenderer::CameraState::FREE:
        freeMove(deltaTime, inputs);
        break;
    case LowRenderer::CameraState::FIRST_PERSON:
        fpsMove(deltaTime, inputs);
        break;
    case LowRenderer::CameraState::LOCK:
        break;
    case LowRenderer::CameraState::FOLLOW:
        followMove();
        break;
    default:
        break;
    }

    pitch = Maths::clamp(pitch, -CAMERA_MAX_PITCH * PI / 180.f, CAMERA_MAX_PITCH * PI / 180.f);
    model.position = {position.x, position.y, position.z};
    model.rotation = { -pitch, yaw, 0.f};
}

mat4x4 LowRenderer::Camera::getViewMatrix()
{
    return  Physics::rotateX(pitch) * Physics::rotateY(-yaw) * Physics::translate(-position.xyz);
    //return mat4::identity();
}

mat4x4 LowRenderer::Camera::getProjectionMatrix()
{
    if (isPerspective)
        return Maths::mat4::perspective(fovY, aspect, near, far);
    else
        return Maths::mat4::orthographic(fovY, aspect, near, far);
}
