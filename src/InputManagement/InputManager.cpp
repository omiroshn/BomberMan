#include "InputManagement/InputManager.hpp"
#include "ResourceManagement/ResourceManager.hpp"
#include "InputManagement/InputManager.hpp"
#include "InputManagement/KeyboardHandler.hpp"
#include "Gui/Gui.h"
#include "Game.hpp"

#include <imgui.h>

InputManager::InputManager()
{
    mJoystick = SDL_JoystickOpen(0);
}

InputManager::~InputManager()
{
    SDL_JoystickClose(mJoystick);
}

void InputManager::processEvents(Game *game, KeyboardHandler &keyHandler)
{
    SDL_Event e;

    while (SDL_PollEvent(&e))
    {
        ImGuiIO &io = ImGui::GetIO();
        switch (e.type)
        {
        case SDL_MOUSEWHEEL:
        {
            if (e.wheel.x > 0)
                io.MouseWheelH += 1;
            if (e.wheel.x < 0)
                io.MouseWheelH -= 1;
            if (e.wheel.y > 0)
                io.MouseWheel += 1;
            if (e.wheel.y < 0)
                io.MouseWheel -= 1;
            break;
        }
        case SDL_MOUSEBUTTONDOWN:
        {
            if (e.button.button == SDL_BUTTON_LEFT)
                Gui::sMousePressed[0] = true;
            if (e.button.button == SDL_BUTTON_RIGHT)
                Gui::sMousePressed[1] = true;
            if (e.button.button == SDL_BUTTON_MIDDLE)
                Gui::sMousePressed[2] = true;
            break;
        }
        case SDL_TEXTINPUT:
        {
            io.AddInputCharactersUTF8(e.text.text);
            break;
        }
        case SDL_KEYDOWN:
        case SDL_KEYUP:
        {
            int key = e.key.keysym.scancode;
            IM_ASSERT(key >= 0 && key < IM_ARRAYSIZE(io.KeysDown));
            io.KeysDown[key] = (e.type == SDL_KEYDOWN);
            io.KeyShift = ((SDL_GetModState() & KMOD_SHIFT) != 0);
            io.KeyCtrl = ((SDL_GetModState() & KMOD_CTRL) != 0);
            io.KeyAlt = ((SDL_GetModState() & KMOD_ALT) != 0);
            io.KeySuper = ((SDL_GetModState() & KMOD_GUI) != 0);
            break;
        }
        }

        switch (e.type)
        {
        case SDL_KEYDOWN:
            game->doAction(processKeyDown(e.key.keysym.sym));
            break;
        case SDL_MOUSEMOTION:
            game->doAction(processMouseMotion(e));
            break;
        case SDL_MOUSEBUTTONDOWN:
            processMouseButton(e.button, true);
            break;
        case SDL_JOYBUTTONDOWN:
            if (e.jbutton.button == SDL_CONTROLLER_BUTTON_START)
                game->doAction(Action::Pause);
            keyHandler.handleJoystickButtonDownEvent(e.jbutton);
            break;
        case SDL_JOYBUTTONUP:
            keyHandler.handleJoystickButtonUpEvent(e.jbutton);
            break;
        case SDL_MOUSEBUTTONUP:
            processMouseButton(e.button, false);
            break;
        case SDL_QUIT:
            game->doAction(Action::Finish);
        default:
            break;
        }
    }
}

Action InputManager::processKeyDown(SDL_Keycode keyPressed)
{
    switch (keyPressed)
    {
        case SDLK_ESCAPE:
            return Action::Pause;
        case SDLK_SPACE:
            return Action::Explosion;
        case SDLK_0:
            return Action::Finish;
        default:
            return Action::Nothing;
    }
}

Action InputManager::processMouseMotion(SDL_Event const &e)
{
    if (mRightButtonPressed)
    {
        mouseOffsetX = float(e.motion.x - prevMousePosX);
        mouseOffsetY = float(prevMousePosY - e.motion.y);

        prevMousePosX = float(e.motion.x);
        prevMousePosY = float(e.motion.y);
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
            prevMousePosX = float(e.x);
            prevMousePosY = float(e.y);
        break;
        default:
            break;
    }
}

SDL_Joystick *InputManager::getJoystick()
{
    return mJoystick;
}
