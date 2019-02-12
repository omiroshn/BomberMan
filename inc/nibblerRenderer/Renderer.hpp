/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Renderer.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkozlov <vkozlov@student.unit.ua>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/08 00:03:00 by vkozlov           #+#    #+#             */
/*   Updated: 2018/07/08 00:03:00 by vkozlov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef RENDERER_H
# define RENDERER_H

#include "GL/glew.h"
#include <SDL.h>

#include "CustomException.hpp"
#include "ResourceManager.hpp"
#include "SpriteRenderer.hpp"
#include <map>

class Renderer {
public:
	Renderer(int width, int height, std::string const &winName, std::string const &binFolder);
	~Renderer();
	std::string		processInput(bool &);
	void			preFrame();
	void			postFrame();
	void			draw(std::string const &texture, glm::vec2 const &pos, glm::vec2 const &scale, float const &rot = 0.0f, glm::vec3 const &color = glm::vec3(1.0f));

private:
	std::shared_ptr<SpriteRenderer>	mSpriteRenderer;
	ResourceManager					mRessourceManager;
	SDL_Window*						mWindow;
	SDL_GLContext					mContext;
	SDL_Event						mE;
private:

	void			init(int width, int height, std::string const &winName, std::string const &binFolder);
	void			deinit();
	Renderer(Renderer const &);
	Renderer &operator=(Renderer const &);
	std::map<int, std::string> mIsBtnPressed;
};

#endif