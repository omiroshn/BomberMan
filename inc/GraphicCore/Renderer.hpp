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
        Bomb,
        Perimeter,
        MAX
    };
}

class Game;
class LightManager;

class Renderer
{
public:
    Renderer();
    ~Renderer();

    void draw(Game&);

    Renderer(Renderer const &) = delete;
    Renderer &operator=(Renderer const &) = delete;
    Camera &getCamera();
	ParticleManager *getParticleManager();
    void updateSize(int x, int y);

private:
    std::vector<glm::mat4> mTransforms[ModelType::MAX];
    void normalPass(Game&);
    void shadowPass(Game&);
    Camera mCamera;
    int mWidth, mHeight;
	ptrPM mParticleManager;
    ptrLM mLightManager;
    void renderObstacles(std::shared_ptr<Shader> &);
    void renderMovable(std::shared_ptr<Shader> &s, std::shared_ptr<Shader> &animated, Game &);
    void prepareTransforms(Game &g);
    int mStage;
};

#endif //BOMBERMAN_Renderer_HPP
