/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Renderer.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkozlov <vkozlov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/08 00:04:00 by vkozlov           #+#    #+#             */
/*   Updated: 2019/02/09 10:46:06 by vkozlov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Renderer.hpp"
#include "Core.h"

namespace {
    glm::vec3 cubePositions[] = {
            glm::vec3( 0.0f,  0.0f,  0.0f),
            glm::vec3( 2.0f,  5.0f, -15.0f),
            glm::vec3(-1.5f, -2.2f, -2.5f),
            glm::vec3(-3.8f, -2.0f, -12.3f),
            glm::vec3( 2.4f, -0.4f, -3.5f),
            glm::vec3(-1.7f,  3.0f, -7.5f),
            glm::vec3( 1.3f, -2.0f, -2.5f),
            glm::vec3( 1.5f,  2.0f, -2.5f),
            glm::vec3( 1.5f,  0.2f, -1.5f),
            glm::vec3(-1.3f,  1.0f, -1.5f)
    };
}

void Renderer::init(std::string const& winName)
{
    initSDL();
    createWindow(winName);
    initGL();
    mapInput();
    initTestData();
}

void            Renderer::initSDL()
{
    SDL_Init(SDL_INIT_EVERYTHING);

    int context_flags = SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG;

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, context_flags);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    SDL_SetRelativeMouseMode(SDL_TRUE);

}

void            Renderer::mapInput()
{
    mIsBtnPressed.emplace(SDLK_w, "UP");
    mIsBtnPressed.emplace(SDLK_s, "DOWN");
    mIsBtnPressed.emplace(SDLK_a, "LEFT");
    mIsBtnPressed.emplace(SDLK_d, "RIGHT");
    mIsBtnPressed.emplace(SDLK_UP, "UP");
    mIsBtnPressed.emplace(SDLK_DOWN, "DOWN");
    mIsBtnPressed.emplace(SDLK_LEFT, "LEFT");
    mIsBtnPressed.emplace(SDLK_RIGHT, "RIGHT");
    mIsBtnPressed.emplace(SDLK_MINUS, "Slower");
    mIsBtnPressed.emplace(SDLK_EQUALS, "Faster");
    mIsBtnPressed.emplace(SDLK_SPACE, "PlaceBomb");
}

void Renderer::initGL()
{
    GLenum err = glewInit();
    if (GLEW_OK != err)
        BM_ASSERT_ALWAYS("Failed to initialize GLEW");
    glViewport(0, 0, mWidth, mHeight);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Renderer::initTestData()
{
    lastX = 0;
    lastY = 0;
    firstMouse = true;

    float vertices[] = {
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
            0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    // world space positions of our cubes

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

}

void Renderer::createWindow(std::string const& winName)
{
    mWindow = SDL_CreateWindow(winName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, mWidth, mHeight, SDL_WINDOW_OPENGL);
    mContext = SDL_GL_CreateContext(mWindow);
    SDL_GL_MakeCurrent(mWindow, mContext);
}

void Renderer::initSpriteRenderer()
{
    glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(mWidth * 2), static_cast<GLfloat>(mHeight * 2), 0.0f, -1.0f, 1.0f);
    auto shader = RESOURCES.getShader("sprite");
    shader->use();
    shader->setMat4("projection", projection);
    mSpriteRenderer = std::make_shared<SpriteRenderer>(shader);
}

std::string Renderer::processInput(bool &isRunning)
{
    std::string command{"nothing"};
    if (mE.type == SDL_KEYDOWN)
    {
        auto find = mIsBtnPressed.find(mE.key.keysym.sym);
        if	(find != mIsBtnPressed.end())
            command = find->second;
        processKeypressEvent(mE);
    }
    if(mE.type == SDL_MOUSEMOTION )
        processMouseMoveEvent(mE);
    if(mE.type == SDL_MOUSEWHEEL )
        processMouseWheelEvent(mE);
    if(mE.type == SDL_QUIT || (mE.type == SDL_KEYDOWN && mE.key.keysym.sym == SDLK_ESCAPE))
        isRunning = false;
    return command;
}

void Renderer::processKeypressEvent(SDL_Event event)
{
    auto key = event.key.keysym.sym;
    switch (key)
    {
        case SDLK_w:
            //fallthrough
        case SDLK_UP:
            cam.movaCamera(CameraDirection::FORWARD, mDeltaTime);
            break;
        case SDLK_s:
            //fallthrough
        case SDLK_DOWN:
            cam.movaCamera(CameraDirection::BACKWARD, mDeltaTime);
            break;
        case SDLK_a:
            //fallthrough
        case SDLK_LEFT:
            cam.movaCamera(CameraDirection::LEFT, mDeltaTime);
            break;
        case SDLK_d:
            //fallthrough
        case SDLK_RIGHT:
            cam.movaCamera(CameraDirection::RIGHT, mDeltaTime);
            break;
        default:
            break;
    }
}

void Renderer::processMouseMoveEvent(SDL_Event event)
{
    std::cout << "x: " << event.motion.x << " y: " << event.motion.y << std::endl;
    if (firstMouse)
    {
        lastX = event.motion.x;
        lastY = event.motion.y;
        firstMouse = false;
    }
    float xoffset = event.motion.x - lastX;
    float yoffset = lastY - event.motion.y; // reversed since y-coordinates go from bottom to top

    lastX = event.motion.x;
    lastY = event.motion.y;
    cam.processMouseMovement(xoffset, yoffset);
}

void Renderer::processMouseWheelEvent(SDL_Event event)
{
    std::cout << "zoom: " << event.wheel.y << std::endl;
    cam.processMouseScroll(event.wheel.y);
}


void Renderer::preFrame()
{
    mLastTime = mCurrentTime;
    mCurrentTime = SDL_GetPerformanceCounter();
    mDeltaTime = (static_cast<float>(mCurrentTime - mLastTime)) * 1000.0f / static_cast<float>(SDL_GetPerformanceFrequency());

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::frame()
{
    RESOURCES.getTexture("block")->bind();

    auto shader = RESOURCES.getShader("modelShader");
    shader->use();
    glm::mat4 projection = glm::perspective(glm::radians(cam.zoom()), static_cast<float>(mWidth) / static_cast<float>(mHeight), 0.1f, 100.0f);
    shader->setMat4("projection", projection);
    glm::mat4 view = cam.getViewMatrix();
    shader->setMat4("view", view);

    // render boxes
    glBindVertexArray(VAO);
    for (unsigned int i = 0; i < 10; i++)
    {
        // calculate the model matrix for each object and pass it to shader before drawing
        glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        model = glm::translate(model, cubePositions[i]);
        float angle = 20.0f * i;
        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        shader->setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}

void Renderer::postFrame()
{
    SDL_GL_SwapWindow(mWindow);
    SDL_PollEvent(&mE);
}

void Renderer::drawSprite(std::string const &tex
        , glm::vec2 const &pos
        , glm::vec2 const &scale
        , float const &rot
        , glm::vec3 const &color)
{
    mSpriteRenderer->drawSprite(RESOURCES.getTexture(tex) , pos, scale, rot, color);
}

void Renderer::deinit()
{
    SDL_GL_DeleteContext(mContext);
    SDL_DestroyWindow(mWindow);
    SDL_Quit();
}

Renderer::Renderer(int width, int height, std::string const &winName) : mWidth(width), mHeight(height), cam(glm::vec3(0.0f, 0.0f, 3.0f))
{
    init(winName);
};

Renderer::~Renderer()
{
    deinit();
};

