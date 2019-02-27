//
// Created by Vadim on 2019-02-12.
//

#ifndef Renderer_HPP
#define Renderer_HPP

#include "GL/glew.h"
#include <SDL.h>
#include "CustomException.hpp"
#include "ResourceManagement/ResourceManager.hpp"
#include "Camera.hpp"

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
public:
    Renderer();
    ~Renderer();

    void draw(std::shared_ptr<MapForRendering>);

    Renderer(Renderer const &) = delete;
    Renderer &operator=(Renderer const &) = delete;
    Camera &getCamera();
    void updateSize(int x, int y);
private:
    void normalPass(std::shared_ptr<MapForRendering>);
    Camera mCamera;
    int mWidth, mHeight;

};

#endif //BOMBERMAN_Renderer_HPP
