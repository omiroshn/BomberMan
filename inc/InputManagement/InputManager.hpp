#ifndef BOMBERMAN_InputManager_HPP
#define BOMBERMAN_InputManager_HPP
#include <SDL.h>
#include "InputManagement/KeyboardHandler.hpp"

//Analog joystick dead zone
const int JOYSTICK_DEAD_ZONE = 8000;
const int MAX_JOYSTICK_VALUE = 32767;

enum class Action
{
    Nothing = 0,
    Finish,
    Pause,
    Forward,
    Backward,
    Up,
    Down,
    Right,
    Left,
    Explosion,
    Space,
    KillAll,
//For internal testing
    CameraRotate
};

class InputManager
{
private:
    SDL_Joystick *mJoystick;
public:
    InputManager();
    ~InputManager();
    InputManager(InputManager const&) = delete;
    InputManager& operator=(InputManager const&) = delete;
    void processEvents(class Game *game, KeyboardHandler &keyHandler);
    Action processKeyDown(SDL_Keycode keyPressed);
    Action processMouseMotion(SDL_Event const &e);
    Action processJoystickMotion(SDL_JoyAxisEvent jaxis);
    Action processJoystickButtonDown(SDL_JoyButtonEvent jbutton);
    void processMouseButton(SDL_MouseButtonEvent const &e, bool isPressed);

    bool mRightButtonPressed {false};
    float prevMousePosX{.0f}, prevMousePosY{.0f};
    float mouseOffsetX{.0f}, mouseOffsetY{.0f};
    void getMouseOffset(float &x, float &y);
    SDL_Joystick *getJoystick();

};

#endif