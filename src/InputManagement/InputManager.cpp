

#include "InputManagement/InputManager.hpp"
#include "ResourceManagement/ResourceManager.hpp"
#include "InputManagement/InputManager.hpp"
#include <imgui.h>
#include "Gui/imgui_impl_sdl_gl3.h"

InputManager::InputManager()
{
}

InputManager::~InputManager()
{
}

Action InputManager::processEvents(SDL_Event &e)
{
    if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE))
        return Action::Finish;

	ImGui_ImplSdlGL3_ProcessEvent(&e);

    switch (e.type)
    {
        case SDL_KEYDOWN:
            return processKeyDown(e.key.keysym.sym);
        case SDL_MOUSEMOTION:
            return processMouseMotion(e);
        case SDL_MOUSEBUTTONDOWN:
            processMouseButton(e.button, true);
            break;
        case SDL_MOUSEBUTTONUP:
            processMouseButton(e.button, false);
            break;
        case SDL_QUIT:
            return Action::Finish;
        default:
            return Action::Nothing;
    }
    return Action::Nothing;
}

Action InputManager::processKeyDown(SDL_Keycode keyPressed)
{
    switch (keyPressed)
    {
        case SDLK_ESCAPE:
            return Action::Finish;
        case SDLK_SPACE:
            return Action::Pause;
        case SDLK_UP:
            //fallthrough
        case SDLK_w:
            return Action::Forward;
        case SDLK_DOWN:
            //fallthrough
        case SDLK_s:
            return Action::Backward;
        case SDLK_LEFT:
            //fallthrough
        case SDLK_a:
            return Action::Left;
        case SDLK_RIGHT:
            //fallthrough
        case SDLK_d:
            return Action::Right;
        case SDLK_q:
            return Action::Up;
        case SDLK_e:
            return Action::Down;
        default:
            return Action::Nothing;
    }
}

Action InputManager::processMouseMotion(SDL_Event const &e)
{
    if (mRightButtonPressed)
    {
        mouseOffsetX = e.motion.x - prevMousePosX;
        mouseOffsetY = prevMousePosY - e.motion.y;

        prevMousePosX = e.motion.x;
        prevMousePosY = e.motion.y;
        return Action::CameraRotate;
    }
    return Action::Nothing;
}

void InputManager::getMouseOffset(float &x, float &y)
{
    x = mouseOffsetX;
    y = mouseOffsetY;
}

void InputManager::processMouseButton(SDL_MouseButtonEvent const &e, bool isPressed)
{
    switch (e.button)
    {
        case SDL_BUTTON_RIGHT:
            mRightButtonPressed = isPressed;
            prevMousePosX = e.x;
            prevMousePosY = e.y;
        break;
        default:
            break;
    }
}
