#include "InputManagement/KeyboardHandler.hpp"
#include "InputManagement/InputManager.hpp"
#include <iostream>

KeyboardHandler::KeyboardHandler()
{
    for (unsigned short i = 0; i < 3; ++i)
        mJButtons[i] = false;
    mKeyboardState = nullptr;
    left_joystick = false;
    right_joystick = false;
}
KeyboardHandler::~KeyboardHandler(){}

void KeyboardHandler::handleKeyboardEvent(SDL_Event &event)
{
    mKeyboardState = SDL_GetKeyboardState(NULL);
}

bool KeyboardHandler::isPressed(int keycode)
{
    if (!mKeyboardState)
        return (false);
    return (mKeyboardState[keycode] == SDL_PRESSED);
}

bool KeyboardHandler::isReleased(int keycode)
{
    if (!mKeyboardState)
        return (false);
    return (mKeyboardState[keycode] == SDL_RELEASED);
}

void KeyboardHandler::handleJoystickEvent(SDL_JoyAxisEvent jaxis)
{
    if (jaxis.axis == 0 || jaxis.axis == 1)
    {
        if (jaxis.value != 0)
            left_joystick = true;
        else
            left_joystick = false;
    }
}

bool KeyboardHandler::LeftJoystickIsActive()
{
    return (left_joystick);
}

void KeyboardHandler::handleJoystickButtonUpEvent(SDL_JoyBallEvent jball)
{
    mJButtons[jball.ball] = false;
}

void KeyboardHandler::handleJoystickButtonDownEvent(SDL_JoyBallEvent jball)
{
    mJButtons[jball.ball] = true;
}

bool KeyboardHandler::JButtonIsPressed(int button)
{
    return (mJButtons[button]);
}
