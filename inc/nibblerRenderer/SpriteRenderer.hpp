/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SpriteRenderer.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkozlov <vkozlov@student.unit.ua>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/12 21:17:00 by vkozlov           #+#    #+#             */
/*   Updated: 2018/07/12 21:17:00 by vkozlov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef NIBBLER_SPRITERENDERER_HPP
#define NIBBLER_SPRITERENDERER_HPP

#include "GL/glew.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <memory>

#include "Texture.hpp"
#include "Shader.hpp"


class SpriteRenderer
{
public:
	// Constructor (inits shaders/shapes)
	SpriteRenderer(std::shared_ptr<Shader> shader);
	// Destructor
	~SpriteRenderer();
	// Renders a defined quad textured with given sprite
	void drawSprite(std::shared_ptr<Texture2D> texture, glm::vec2 position, glm::vec2 size = glm::vec2(10, 10), GLfloat rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f));
private:
	SpriteRenderer();
	SpriteRenderer(SpriteRenderer const &);
	SpriteRenderer &operator=(SpriteRenderer const &);

	// Render state
	std::shared_ptr<Shader> mShader;
	GLuint mQuadVAO;
	// Initializes and configures the quad's buffer and vertex attributes
	void initRenderData();
};
#endif //NIBBLER_SPRITERENDERER_HPP
