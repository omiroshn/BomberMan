#include "InputManagement/InputManager.hpp"
#include "ResourceManagement/ResourceManager.hpp"
#include "InputManagement/InputManager.hpp"
#include <imgui.h>
#include "InputManagement/KeyboardHandler.hpp"
#include "Gui/Gui.h"

InputManager::InputManager()
{
    if (SDL_NumJoysticks() < 1)
        std::cout << "Warning: No joysticks connected!" << std::endl;

    mJoystick = SDL_JoystickOpen(0);
    if (mJoystick == NULL)
        printf("Warning: Unable to open game controller! SDL Error: %s\n", SDL_GetError());
    else
    {
        std::cout << "Connected joysticks: " << SDL_NumJoysticks() << std::endl;
        std::cout << "Controller name: " << SDL_JoystickName(mJoystick) << std::endl;
        std::cout << "Num axes: " << SDL_JoystickNumAxes(mJoystick) << std::endl;
        std::cout << "Num buttons: " << SDL_JoystickNumButtons(mJoystick) << std::endl;
    }
}

InputManager::~InputManager()
{
    SDL_JoystickClose(mJoystick);
}

Action InputManager::processEvents(SDL_Event &e, KeyboardHandler &keyHandler)
{
    if (e.type == SDL_QUIT)
        return Action::Finish;

    ImGuiIO& io = ImGui::GetIO();
    switch (e.type)
    {
    case SDL_MOUSEWHEEL:
        {
            if (e.wheel.x > 0) io.MouseWheelH += 1;
            if (e.wheel.x < 0) io.MouseWheelH -= 1;
            if (e.wheel.y > 0) io.MouseWheel += 1;
            if (e.wheel.y < 0) io.MouseWheel -= 1;
			break;
        }
    case SDL_MOUSEBUTTONDOWN:
        {
            if (e.button.button == SDL_BUTTON_LEFT) Gui::sMousePressed[0] = true;
            if (e.button.button == SDL_BUTTON_RIGHT) Gui::sMousePressed[1] = true;
            if (e.button.button == SDL_BUTTON_MIDDLE) Gui::sMousePressed[2] = true;
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
        case SDL_KEYUP:
            keyHandler.handleKeyboardEvent(e);
            break;
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
        case SDL_JOYAXISMOTION:
            keyHandler.handleJoystickEvent(e.jaxis);
            return processJoystickMotion(e.jaxis);
        case SDL_JOYBUTTONDOWN:
            keyHandler.handleJoystickButtonDownEvent(e.jball);
            return processJoystickButtonDown(e.jbutton);
        case SDL_JOYBUTTONUP:
            keyHandler.handleJoystickButtonUpEvent(e.jball);
            break;
        case SDL_QUIT:
            return Action::Finish;
        default:
            return Action::Nothing;
    }
    return Action::Nothing;
}

Action InputManager::processJoystickMotion(SDL_JoyAxisEvent jaxis)
{
    if (jaxis.which == 0)
    {
        return Action::Joystick;
    }
    return Action::Nothing;
}

Action InputManager::processJoystickButtonDown(SDL_JoyButtonEvent jbutton)
{
    if (jbutton.which == 0)
    {
        if (jbutton.button == 2)
            return Action::JoystickButtonX;
        else if (jbutton.button == 1)
            return Action::JoystickButtonY;
    }
    return Action::Nothing;
}

Action InputManager::processKeyDown(SDL_Keycode keyPressed)
{
    switch (keyPressed)
    {
        case SDLK_ESCAPE:
            return Action::Pause;
        case SDLK_SPACE:
            return Action::Explosion;
        case SDLK_1:
            return Action::StageFinished;
        case SDLK_UP:
            return Action::Up;
        case SDLK_w:
            return Action::Forward;
        case SDLK_DOWN:
            return Action::HeroDied;
        case SDLK_s:
            return Action::Backward;
        case SDLK_LEFT:
            return Action::Left;
        case SDLK_a:
            return Action::Left;
        case SDLK_RIGHT:
            return Action::Right;
        case SDLK_d:
            return Action::Right;
        case SDLK_q:
            return Action::Up;
        case SDLK_e:
            return Action::Down;
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
