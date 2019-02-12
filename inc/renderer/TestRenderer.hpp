//
// Created by Vadim on 2019-02-12.
//

#ifndef BOMBERMAN_TESTRENDERER_HPP
#define BOMBERMAN_TESTRENDERER_HPP

#include "GL/glew.h"
#include <SDL.h>

#include "CustomException.hpp"
#include "SpriteRenderer.hpp"
#include "NewResourceManager.hpp"
#include <map>

class TestRenderer {
public:
    TestRenderer(int width, int height, std::string const &winName);
    ~TestRenderer();
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
    TestRenderer(TestRenderer const &);
    TestRenderer &operator=(TestRenderer const &);
    std::map<int, std::string> mIsBtnPressed;
    int mWidth, mHeight;
};

#endif //BOMBERMAN_TESTRENDERER_HPP
