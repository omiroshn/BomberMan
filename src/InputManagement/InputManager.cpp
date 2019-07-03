

#include "InputManagement/InputManager.hpp"
#include "ResourceManagement/ResourceManager.hpp"
#include "InputManagement/InputManager.hpp"
#include <imgui.h>
#include "Gui/imgui_impl_sdl_gl3.h"

int xDir = 0;
int yDir = 0;

InputManager::InputManager()
{
}

InputManager::~InputManager()
{
}

Action InputManager::processEvents(SDL_Event &e)
{
    if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE))
        return Action::Finish;

	ImGui_ImplSdlGL3_ProcessEvent(&e);

    switch (e.type)
    {
        case SDL_KEYDOWN:
            return processKeyDown(e.key.keysym.sym);
        case SDL_MOUSEMOTION:
            return processMouseMotion(e);
        case SDL_MOUSEBUTTONDOWN:
            processMouseButton(e.button, true);
            break;
        case SDL_MOUSEBUTTONUP:
            processMouseButton(e.button, false);
            break;
        case SDL_JOYAXISMOTION:
            return processJoystickMotion(e.jaxis);
        case SDL_JOYBUTTONDOWN:
            return processJoystickButtonDown(e.jbutton);
        case SDL_QUIT:
            return Action::Finish;
        default:
            return Action::Nothing;
    }
    return Action::Nothing;
}

Action InputManager::processJoystickMotion(SDL_JoyAxisEvent jaxis)
{
    if (jaxis.which == 0)
    {
        if (jaxis.axis == 0)
        {
            
            if (jaxis.value < -JOYSTICK_DEAD_ZONE)
                xDir = -1;
            else if (jaxis.value > JOYSTICK_DEAD_ZONE)
                xDir = 1;
            else
                xDir = 0;
        }
        else if (jaxis.axis == 1)
        {
            if (jaxis.value < -JOYSTICK_DEAD_ZONE)
                yDir = -1;
            else if (jaxis.value > JOYSTICK_DEAD_ZONE)
                yDir = 1;
            else
                yDir = 0;
        }
        double joystickAngle = atan2((double)yDir, (double)xDir) * (180.0 / M_PI);
        
        if (xDir == 0 && yDir == 0)
            joystickAngle = 0;
        if (joystickAngle == 0 && xDir == 1)
            return Action::Right;
        else if (joystickAngle == 45)
            return Action::DownRight;
        else if (joystickAngle == 90)
            return Action::Backward;
        else if (joystickAngle == 135)
            return Action::DownLeft;
        else if (joystickAngle == 180)
            return Action::Left;
        else if (joystickAngle == -135)
            return Action::UpLeft;
        else if (joystickAngle == -90)
            return Action::Forward;
        else if (joystickAngle == -45)
            return Action::UpRight;
    }
    return Action::Nothing;
}

Action InputManager::processJoystickButtonDown(SDL_JoyButtonEvent jbutton)
{
    if (jbutton.which == 0)
    {
        if (jbutton.button == 2)
            return Action::Down;
        else if (jbutton.button == 1)
            return Action::Up;
    }
    return Action::Nothing;
}

Action InputManager::processKeyDown(SDL_Keycode keyPressed)
{
    switch (keyPressed)
    {
        case SDLK_ESCAPE:
            return Action::Finish;
        case SDLK_SPACE:
            return Action::Pause;
        case SDLK_UP:
            //fallthrough
        case SDLK_w:
            return Action::Forward;
        case SDLK_DOWN:
            //fallthrough
        case SDLK_s:
            return Action::Backward;
        case SDLK_LEFT:
            //fallthrough
        case SDLK_a:
            return Action::Left;
        case SDLK_RIGHT:
            //fallthrough
        case SDLK_d:
            return Action::Right;
        case SDLK_q:
            return Action::Up;
        case SDLK_e:
            return Action::Down;
        default:
            return Action::Nothing;
    }
}

Action InputManager::processMouseMotion(SDL_Event const &e)
{
    if (mRightButtonPressed)
    {
        mouseOffsetX = e.motion.x - prevMousePosX;
        mouseOffsetY = prevMousePosY - e.motion.y;

        prevMousePosX = e.motion.x;
        prevMousePosY = e.motion.y;
        return Action::CameraRotate;
    }
    return Action::Nothing;
}

void InputManager::getMouseOffset(float &x, float &y)
{
    x = mouseOffsetX;
    y = mouseOffsetY;
}

void InputManager::processMouseButton(SDL_MouseButtonEvent const &e, bool isPressed)
{
    switch (e.button)
    {
        case SDL_BUTTON_RIGHT:
            mRightButtonPressed = isPressed;
            prevMousePosX = e.x;
            prevMousePosY = e.y;
        break;
        default:
            break;
    }
}
