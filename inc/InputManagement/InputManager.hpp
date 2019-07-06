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
    StageFinished,
    Forward,
    Backward,
    Up,
    Down,
    Right,
    Left,
    Joystick,
    JoystickButtonX,
    JoystickButtonY,
    JoystickButtonA,
    JoystickButtonB,
    Explosion,
    Space,
//For internal testing
    CameraRotate,
    CameraZoom
};

class InputManager
{
private:
    SDL_Joystick *mJoystick;
public:
    InputManager();
    ~InputManager();
    Action processEvents(SDL_Event &e, KeyboardHandler &keyHandler);
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