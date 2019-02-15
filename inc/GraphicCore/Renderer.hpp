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

#pragma region example
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

#pragma endregion

class Renderer {
public:
    Renderer();
    ~Renderer();

    void draw(/*std::vector<Instance> const& whatToDraw*/);

    Renderer(Renderer const &) = delete;
    Renderer &operator=(Renderer const &) = delete;

private:
    void normalPass(/*std::vector<Instance> const& instances*/);
};

#endif //BOMBERMAN_Renderer_HPP
