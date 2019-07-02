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
	struct Quad {
		glm::vec3 topLeft;
		glm::vec3 bottomRight;
		Shader shader;
		Texture* texture = nullptr;
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
    void drawQuadsDeferred();
    Camera mCamera;
    int mWidth, mHeight;
	ptrPM mParticleManager;
	
	std::vector<Quad> mQuads;


};

#endif //BOMBERMAN_Renderer_HPP
