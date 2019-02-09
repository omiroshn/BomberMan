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
#ifndef DRAWER_H
# define DRAWER_H

#include "GL/glew.h"
#include <SDL.h>

#include "CustomException.hpp"
#include "ResourceManager.hpp"
#include "SpriteRenderer.hpp"
#include <map>

class Drawer {
public:
	Drawer(int width, int height, std::string const &winName);
	~Drawer();
	std::string		processInput(bool &);
	void			preFrame();
	void			postFrame();
	void			draw(std::string const &texture, glm::vec2 const &pos, glm::vec2 const &scale, float const &rot = 0.0f, glm::vec3 const &color = glm::vec3(1.0f));
//	void			putString(std::string const&what,glm::vec2 where, float size, glm::vec3 color);
private:
	int								mWidth, mHeight;
	std::shared_ptr<SpriteRenderer>	mSpriteRenderer;
	ResourceManager					mRessourceManager;
	SDL_Window*						mWindow;
	SDL_GLContext					mContext;
	SDL_Event						mE;
private:

	void			init(int width, int height, std::string const &winName);
	void			deinit();
	Drawer(Drawer const &);
	Drawer &operator=(Drawer const &);
	std::map<int, std::string> mIsBtnPressed;
};

#endif