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
    mKeyboardState = SDL_GetKeyboardState(NULL);
}
KeyboardHandler::~KeyboardHandler(){}

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

void KeyboardHandler::handleJoystickButtonUpEvent(SDL_JoyButtonEvent jball)
{
    mJButtons[jball.button] = false;
}

void KeyboardHandler::handleJoystickButtonDownEvent(SDL_JoyButtonEvent jball)
{
    mJButtons[jball.button] = true;
}

bool KeyboardHandler::JButtonIsPressed(int button)
{
    return (mJButtons[button]);
}
