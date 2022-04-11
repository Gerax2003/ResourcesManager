#pragma once
#include "Core/Model.hpp"

namespace LowRenderer
{
    struct CameraInputs
    {
        float deltaX;
        float deltaY;
        bool moveForward;
        bool moveBackward;
        bool moveLeft;
        bool moveRight;
        bool moveUpwards;
        bool moveDownwards;
        // ... and more if needed
    };

    enum class CameraState
    {
        FREE,
        FIRST_PERSON,
        LOCK,
        FOLLOW
    };

    struct Camera
    {
        Camera(int width, int height);

        void update(float deltaTime, const CameraInputs& inputs);
        void freeMove(float deltaTime, const CameraInputs& inputs);
        void fpsMove(float deltaTime, const CameraInputs& inputs);
        void followMove();
        mat4x4 getViewMatrix();
        mat4x4 getProjectionMatrix();
        
        CameraState state = CameraState::FOLLOW;
        Model model = Model();
        Model* toFollow = nullptr;

        float yaw = 0;
        float pitch = 0.f;
        Vector4 position = { 0, 3.5f, 9.0f, 0 };
        Vector3 offsetFromTarget = {0.f, 3.5f, 9.f};

        float aspect;
        float fovY = 90;
        float near = 0.01f; // TODO: if near is lower, everything breaks
        float far = 80000.f;

        bool isPerspective = true;
    };
}