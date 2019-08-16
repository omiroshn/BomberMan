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

void KeyboardHandler::handleKeyboardEvent(SDL_Event &)
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

void KeyboardHandler::handleJoystickHatEvent(SDL_JoyHatEvent jhat)
{
    static int last_val = -1;

    if ((int)jhat.value == 0)
        mJHatButtons[last_val] = false;
    else
    {
        last_val = (int)jhat.value;
        mJHatButtons[(int)jhat.value] = true;
    }
}

bool KeyboardHandler::isHatPressed(int keycode)
{
    return (mJHatButtons[keycode] == SDL_PRESSED);
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

void KeyboardHandler::handleJoystickButtonUpEvent(SDL_JoyButtonEvent jbutton)
{
    mJButtons[jbutton.button] = false;
}

void KeyboardHandler::handleJoystickButtonDownEvent(SDL_JoyButtonEvent jbutton)
{
    mJButtons[jbutton.button] = true;
}

bool KeyboardHandler::JButtonIsPressed(int button)
{
    return (mJButtons[button]);
}
