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

void Renderer::init(int width, int height, std::string const &winName)
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

	mWindow = SDL_CreateWindow(winName.c_str(),SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
	mContext = SDL_GL_CreateContext(mWindow);
	SDL_GL_MakeCurrent(mWindow, mContext);
    GLenum err = glewInit();
    if (GLEW_OK != err)
        throw CustomException("Failed to initialize GLEW");
	glViewport(0, 0, width, height);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	auto shader = mRessourceManager.loadShader("Assets/shader/sprite.vx.glsl", "Assets/shader/sprite.ft.glsl", "sprite");
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(width * 2), static_cast<GLfloat>(height * 2), 0.0f, -1.0f, 1.0f);
	shader->use();
	shader->setInt("image", 0);
	shader->setMat4("projection", projection);
	mSpriteRenderer = std::make_shared<SpriteRenderer>(shader);
	mRessourceManager.loadTexture("Assets/img/block.png", "block", false);

	auto textShader = mRessourceManager.loadShader("Assets/shader/text.vx.glsl", "Assets/shader/text.ft.glsl", "text");

	projection = glm::ortho(0.0f, static_cast<GLfloat>(width), static_cast<GLfloat>(height), 0.0f);

	textShader->use();
	textShader->setInt("text", 0);
	textShader->setMat4("projection", projection);

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
	mIsBtnPressed.emplace(SDLK_SPACE, "Pause");
	mIsBtnPressed.emplace(SDLK_1, "LIB1");
	mIsBtnPressed.emplace(SDLK_2, "LIB2");
	mIsBtnPressed.emplace(SDLK_3, "LIB3");
}

std::string Renderer::processInput(bool &isRunning)
{
	std::string command{"nothing"};
	if (mE.type == SDL_KEYDOWN)
	{
		auto find = mIsBtnPressed.find(mE.key.keysym.sym);
		if	(find != mIsBtnPressed.end())
			command = find->second;
	}
	if(mE.type == SDL_QUIT || (mE.type == SDL_KEYDOWN && mE.key.keysym.sym == SDLK_ESCAPE))
		isRunning = false;
	return command;
}

void Renderer::preFrame()
{
	glClearColor(0.f, 0.f, 0.f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::postFrame()
{
	SDL_GL_SwapWindow(mWindow);
	SDL_PollEvent(&mE);
}

void Renderer::draw(std::string const &tex
		, glm::vec2 const &pos
		, glm::vec2 const &scale
		, float const &rot
		, glm::vec3 const &color)
{
	mSpriteRenderer->drawSprite(mRessourceManager.getTexture(tex) , pos, scale, rot, color);
}

void Renderer::deinit()
{
	SDL_GL_DeleteContext(mContext);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}

Renderer::Renderer(int width, int height, std::string const &winName)
{
	init(width, height, winName);
};

Renderer::~Renderer()
{
	deinit();
};

Renderer::Renderer(Renderer const &) {};
Renderer &Renderer::operator=(Renderer const &) { return *this; };
