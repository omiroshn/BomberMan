#ifndef BOMBERMAN_InputManager_HPP
#define BOMBERMAN_InputManager_HPP
#include <SDL.h>

enum class Action
{
    Finish = 0,
    Up,
    Down,
    Right,
    Left,
    Space
};

class InputManager
{
public:
    InputManager();
    ~InputManager();
    Action processEvents(SDL_Event const &e);
};

#endif