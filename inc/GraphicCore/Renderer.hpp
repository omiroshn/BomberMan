//
// Created by Vadim on 2019-02-12.
//

#ifndef Renderer_HPP
#define Renderer_HPP

#include "GL/glew.h"
#include <SDL.h>
#include "CustomException.hpp"
#include "ResourceManagement/ResourceManager.hpp"
#include "ResourceManagement/ParticleManager.hpp"
#include "Camera.hpp"

typedef std::unique_ptr<ParticleManager> ptrPM;

enum class ModelType
{
    Wall = 0,
    Brick,
    Player,
    EnemyType1,
    Bonus
};

struct Instance
{
    ModelType t;
    glm::vec3 position;
};


class MapForRendering;
class Renderer
{
	struct Vert
	{
		glm::vec3 pos;
		glm::vec2 uv;
	};
	struct Quad {
		Vert v[6];
		Quad(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d)
			:
			v{
				{a,{0,0}},
				{b,{0,1}},
				{c,{1,0}},
				{c,{1,0}},
				{b,{0,1}},
				{d,{1,1}}
			} {}
	};
public:
    Renderer();
    ~Renderer();

    void draw(MapForRendering&);
    void drawQuad(Quad);
    void drawPicture(const std::string& pic);

    Renderer(Renderer const &) = delete;
    Renderer &operator=(Renderer const &) = delete;
    Camera &getCamera();
	ParticleManager *getParticleManager();
    void updateSize(int x, int y);

private:
    void normalPass(MapForRendering&);
    void drawShadow(glm::vec3 position);
    void drawQuadsDeferred(glm::mat4 view, glm::mat4 projection);
    Camera mCamera;
    int mWidth, mHeight;
	ptrPM mParticleManager;
	
	std::vector<Quad> mShadowQuads;
	GLuint	mQuadsBuffer;
	GLuint	mQuadsArray;

};

#endif //BOMBERMAN_Renderer_HPP
