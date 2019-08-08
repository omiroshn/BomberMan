#ifndef BOMBERMAN_KeyboardHandler_HPP
#define BOMBERMAN_KeyboardHandler_HPP
#include <SDL.h>

class KeyboardHandler
{
private:
    const Uint8 *mKeyboardState;
    bool  left_joystick;
    bool right_joystick;
    bool mJButtons[4];
    
public:
    KeyboardHandler();
    ~KeyboardHandler();
    KeyboardHandler(KeyboardHandler const&) = delete;
    KeyboardHandler& operator=(KeyboardHandler const&) = delete;

    bool isPressed(int keycode);
    bool isReleased(int keycode);
    void handleKeyboardEvent(SDL_Event &event);
    void handleJoystickEvent(SDL_JoyAxisEvent jaxis);
    void handleJoystickButtonUpEvent(SDL_JoyBallEvent jball);
    void handleJoystickButtonDownEvent(SDL_JoyBallEvent jball);
    bool LeftJoystickIsActive();
    bool JButtonIsPressed(int button);
};

#endif