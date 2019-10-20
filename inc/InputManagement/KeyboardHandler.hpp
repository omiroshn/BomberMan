#ifndef BOMBERMAN_KeyboardHandler_HPP
#define BOMBERMAN_KeyboardHandler_HPP
#include <SDL.h>

class KeyboardHandler
{
private:
    const Uint8 *mKeyboardState;
    bool  left_joystick;
    bool right_joystick;
    bool mJButtons[SDL_CONTROLLER_BUTTON_MAX] = {0};
    
public:
    KeyboardHandler();
    ~KeyboardHandler();
    KeyboardHandler(KeyboardHandler const&) = delete;
    KeyboardHandler& operator=(KeyboardHandler const&) = delete;

    bool isPressed(int keycode);
    bool isReleased(int keycode);
    void handleJoystickEvent(SDL_JoyAxisEvent jaxis);
    void handleJoystickButtonUpEvent(SDL_JoyButtonEvent jball);
    void handleJoystickButtonDownEvent(SDL_JoyButtonEvent jball);
    bool JButtonIsPressed(int button);
};

#endif