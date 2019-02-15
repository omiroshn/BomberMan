

#include "InputManagement/InputManager.hpp"
#include "ResourceManagement/ResourceManager.hpp"
#include "InputManagement/InputManager.hpp"

InputManager::InputManager()
{
}

InputManager::~InputManager()
{
}

Action InputManager::processEvents(SDL_Event const &e)
{
    if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE))
        return Action::Finish;

    switch (e.type)
    {
        case SDL_KEYDOWN:
            return processKeyDown(e.key.keysym.sym);
        case SDL_MOUSEMOTION:
            return processMouseMotion(e);
        case SDL_QUIT:
            return Action::Finish;
        default:
            return Action::Nothing;
    }
}

Action InputManager::processKeyDown(SDL_Keycode keyPressed)
{
    switch (keyPressed)
    {
        case SDLK_ESCAPE:
            return Action::Finish;
        default:
            return Action::Nothing;
    }
}

Action InputManager::processMouseMotion(SDL_Event const &e)
{
    mouseOffsetX = e.motion.x - prevMousePosX;
    mouseOffsetY = prevMousePosY - e.motion.y;

    prevMousePosX = e.motion.x;
    prevMousePosY = e.motion.y;
    return Action::CameraRotate;
}

void InputManager::getMouseOffset(float &x, float &y)
{
    x = mouseOffsetX;
    y = mouseOffsetY;
}
