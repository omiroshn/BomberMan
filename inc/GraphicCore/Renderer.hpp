//
// Created by Vadim on 2019-02-12.
//

#ifndef Renderer_HPP
#define Renderer_HPP
#include <unordered_map>

#include "GL/glew.h"
#include <SDL.h>
#include "CustomException.hpp"
#include "ResourceManagement/ResourceManager.hpp"
#include "ResourceManagement/ParticleManager.hpp"
#include "Camera.hpp"
#include "GraphicCore/LightManager.hpp"

typedef std::unique_ptr<ParticleManager> ptrPM;
namespace ModelType{
    enum 
    {
        Wall = 0,
        Brick,
        Player,
        EnemyType1,
        EnemyType2,
        EnemyType3,
        Bonus,
        Bomb,
        Perimeter,
        MAX
    };
}

class Game;
class LightManager;
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

    void draw(Game&);
    void drawQuad(Quad);

    Renderer(Renderer const &) = delete;
    Renderer &operator=(Renderer const &) = delete;
    Camera &getCamera();
	ParticleManager *getParticleManager();
    void updateSize(int x, int y);

private:
    std::vector<glm::mat4> mTransforms[ModelType::MAX];
    void normalPass(Game&);
    void shadowPass(Game&);
    void drawShadow(glm::vec3 position);
    void drawQuadsDeferred(glm::mat4 view, glm::mat4 projection);
    Camera mCamera;
    int mWidth, mHeight;
	ptrPM mParticleManager;
    ptrLM mLightManager;
    void renderObstacles(std::shared_ptr<Shader> &);
    void renderMovable(std::shared_ptr<Shader> &, Game &);
    void prepareTransforms(Game &g);
    int mStage;
};

#endif //BOMBERMAN_Renderer_HPP
