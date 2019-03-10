#ifndef BOMBERMAN_InputManager_HPP
#define BOMBERMAN_InputManager_HPP
#include <SDL.h>

enum class Action
{
    Nothing = 0,
    Finish,
    Forward,
    Backward,
    Up,
    Down,
    Right,
    Left,
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
    void processMouseButton(SDL_MouseButtonEvent const &e, bool isPressed);

    bool mRightButtonPressed {false};
    float prevMousePosX{.0f}, prevMousePosY{.0f};
    float mouseOffsetX{.0f}, mouseOffsetY{.0f};
    void getMouseOffset(float &x, float &y);

};

#endif