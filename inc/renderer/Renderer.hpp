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
#include <map>

class Renderer {
public:
    Renderer(int width, int height, std::string const &winName);
    ~Renderer();
    std::string		processInput(bool &);
    void			preFrame();
    void			postFrame();
    void			draw(std::string const &texture, glm::vec2 const &pos, glm::vec2 const &scale, float const &rot = 0.0f, glm::vec3 const &color = glm::vec3(1.0f));
    void                            setProjection();

private:
    std::shared_ptr<SpriteRenderer>	mSpriteRenderer;
    SDL_Window*						mWindow;
    SDL_GLContext					mContext;
    SDL_Event						mE;
private:
    void			init(int width, int height, std::string const &winName);
    void			deinit();
    Renderer(Renderer const &);
    Renderer &operator=(Renderer const &);
    std::map<int, std::string> mIsBtnPressed;
    int mWidth, mHeight;
};

#endif //BOMBERMAN_Renderer_HPP
