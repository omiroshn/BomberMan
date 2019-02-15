#ifndef BOMBERMAN_InputManager_HPP
#define BOMBERMAN_InputManager_HPP
#include <SDL.h>

enum class Action
{
    Nothing = 0,
    Finish,
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
    Action processEvents(SDL_Event const &e);
    Action processKeyDown(SDL_Keycode keyPressed);
    Action processMouseMotion(SDL_Event const &e);



    float prevMousePosX{.0f}, prevMousePosY{.0f};
    float mouseOffsetX{.0f}, mouseOffsetY{.0f};
    void getMouseOffset(float &x, float &y);

};

#endif