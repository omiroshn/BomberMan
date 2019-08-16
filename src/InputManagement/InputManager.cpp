

#include "InputManagement/InputManager.hpp"
#include "ResourceManagement/ResourceManager.hpp"
#include "InputManagement/InputManager.hpp"
#include <imgui.h>
#include "Gui/imgui_impl_sdl_gl3.h"
#include "InputManagement/KeyboardHandler.hpp"

InputManager::InputManager()
{
    if (SDL_NumJoysticks() < 1)
        std::cout << "Warning: No joysticks connected!" << std::endl;

    mJoystick = SDL_JoystickOpen(0);
    if (mJoystick == NULL)
        printf("Warning: Unable to open game controller! SDL Error: %s\n", SDL_GetError());
    else
    {
        std::cout << "Connected joysticks: " << SDL_NumJoysticks() << std::endl;
        std::cout << "Controller name: " << SDL_JoystickName(mJoystick) << std::endl;
        std::cout << "Num axes: " << SDL_JoystickNumAxes(mJoystick) << std::endl;
        std::cout << "Num buttons: " << SDL_JoystickNumButtons(mJoystick) << std::endl;
    }
}

InputManager::~InputManager()
{
    SDL_JoystickClose(mJoystick);
}

Action InputManager::processEvents(SDL_Event &e, KeyboardHandler &keyHandler)
{
    if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE))
        return Action::Finish;

	ImGui_ImplSdlGL3_ProcessEvent(&e);

    switch (e.type)
    {
        case SDL_KEYUP:
            keyHandler.handleKeyboardEvent(e);
            break;
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
            keyHandler.handleJoystickEvent(e.jaxis);
            return processJoystickMotion(e.jaxis);
        case SDL_JOYBUTTONDOWN:
            keyHandler.handleJoystickButtonDownEvent(e.jball);
            return processJoystickButtonDown(e.jbutton);
        case SDL_JOYBUTTONUP:
            keyHandler.handleJoystickButtonUpEvent(e.jball);
            break;
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
        return Action::Joystick;
    }
    return Action::Nothing;
}

Action InputManager::processJoystickButtonDown(SDL_JoyButtonEvent jbutton)
{
    if (jbutton.which == 0)
    {
        if (jbutton.button == 2)
            return Action::JoystickButtonX;
        else if (jbutton.button == 1)
            return Action::JoystickButtonY;
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
            return Action::Explosion;
        case SDLK_1:
            return Action::StageFinished;
        case SDLK_UP:
            return Action::Up;
        case SDLK_w:
            return Action::Forward;
        case SDLK_DOWN:
            return Action::HeroDied;
        case SDLK_s:
            return Action::Backward;
        case SDLK_LEFT:
            return Action::Left;
        case SDLK_a:
            return Action::Left;
        case SDLK_RIGHT:
            return Action::Right;
        case SDLK_d:
            return Action::Right;
        case SDLK_q:
            return Action::Up;
        case SDLK_e:
            return Action::Down;
        case SDLK_0:
            return Action::Pause;
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

SDL_Joystick *InputManager::getJoystick()
{
    return mJoystick;
}