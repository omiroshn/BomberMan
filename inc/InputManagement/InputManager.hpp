#ifndef BOMBERMAN_InputManager_HPP
#define BOMBERMAN_InputManager_HPP
#include <SDL.h>

#define PRINT(x) std::cout << #x << " = " << x << "\n"

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
    UpLeft,
    UpRight,
    DownLeft,
    DownRight,
    Space,
//For internal testing
    CameraRotate,
    CameraZoom
};

class InputManager
{
public:
    InputManager();
    ~InputManager();
    Action processEvents(SDL_Event &e);
    Action processKeyDown(SDL_Keycode keyPressed);
    Action processMouseMotion(SDL_Event const &e);
    Action processJoystickMotion(SDL_JoyAxisEvent jaxis);
    Action processJoystickButtonDown(SDL_JoyButtonEvent jbutton);
    void processMouseButton(SDL_MouseButtonEvent const &e, bool isPressed);

    bool mRightButtonPressed {false};
    float prevMousePosX{.0f}, prevMousePosY{.0f};
    float mouseOffsetX{.0f}, mouseOffsetY{.0f};
    void getMouseOffset(float &x, float &y);

};

#endif