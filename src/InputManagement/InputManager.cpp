

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
}
