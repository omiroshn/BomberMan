//
// Created by Vadim on 2019-02-12.
//

#ifndef BOMBERMAN_Renderer_HPP
#define BOMBERMAN_Renderer_HPP

#include "GL/glew.h"
#include <SDL.h>

#include "CustomException.hpp"
#include "SpriteRenderer.hpp"
#include "ResourceManager.hpp"
#include "Camera.hpp"
#include <map>

class Renderer {
public:
    Renderer(int width, int height, std::string const &winName);
    ~Renderer();
    std::string		processInput(bool &);
    void			preFrame();
    void            frame();
    void			postFrame();
    void			drawSprite(std::string const &texture, glm::vec2 const &pos, glm::vec2 const &scale, float const &rot = 0.0f, glm::vec3 const &color = glm::vec3(1.0f));
    void            initSpriteRenderer();

    Renderer(Renderer const &) = delete;
    Renderer &operator=(Renderer const &) = delete;

private:
    std::shared_ptr<SpriteRenderer>	mSpriteRenderer;
    SDL_Window*						mWindow;
    SDL_GLContext					mContext;
    SDL_Event						mE;
private:
    void			init(std::string const &winName);
    void			deinit();
    void            initSDL();
    void            initGL();
    void            createWindow(std::string const& winName);
    void            mapInput();

    //events
private:
    void processKeypressEvent(SDL_Event e);
    void processMouseMoveEvent(SDL_Event e);
    void processMouseWheelEvent(SDL_Event e);
    std::map<int, std::string> mIsBtnPressed;
    int mWidth, mHeight;
    Camera cam;


    //deltaTime
    Uint64 mCurrentTime;
    Uint64 mLastTime;
    float mDeltaTime;

    //test data
    void initTestData();
    unsigned int VBO, VAO;

    float lastX;
    float lastY;
    bool firstMouse;

};

#endif //BOMBERMAN_Renderer_HPP
