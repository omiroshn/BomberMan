#include "GraphicCore/Renderer.hpp"
#include "LogicCore/MapForRendering.h"
#include "ResourceManagement/Texture.hpp"
#include "ResourceManagement/Shader.hpp"
#include "ResourceManagement/Model.hpp"
#include "ResourceManagement/Skybox.hpp"

Renderer::Renderer() : mCamera(glm::vec3(0.0f, 10.0f, -3.0f))
{

};

Renderer::~Renderer()
{
};

void Renderer::updateSize(int aWidth, int aHeight)
{
    mWidth = aWidth;
    mHeight = aHeight;
}

void Renderer::draw(std::shared_ptr<MapForRendering> aMap)
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    normalPass(aMap);
}

void Renderer::normalPass(std::shared_ptr<MapForRendering> aMap)
{
    glViewport(0, 0, mWidth, mHeight);

    auto brick = RESOURCES.getModel("brick");
    auto wall = RESOURCES.getModel("wall");
    auto suite = RESOURCES.getModel("nanosuit");
    auto ground = RESOURCES.getModel("ground");
    auto skybox = RESOURCES.getSkybox("defaultSkybox");
    auto modelShader = RESOURCES.getShader("modelShader");
    auto skyboxShader = RESOURCES.getShader("skybox");

    modelShader->use();
    glm::mat4 projection = glm::perspective(glm::radians(mCamera.zoom()), static_cast<float>(mWidth) / static_cast<float>(mHeight), 0.1f, 100.0f);
    glm::mat4 view = mCamera.getViewMatrix();

    modelShader->setMat4("projection", projection);
    modelShader->setMat4("view", view);
    modelShader->setVec3("lightPos", mCamera.position());

    std::vector<glm::mat4> transforms;

    // render the ground
    {
        glm::mat4 groundModel = glm::mat4(1.0f);
        groundModel = glm::translate(groundModel, glm::vec3(9.5, -0.5f, 9.5));
        groundModel = glm::scale(groundModel, glm::vec3(20.0f, 0.1f, 20.0f));
        transforms.push_back(groundModel);
        ground->draw(modelShader, transforms);
    }

    transforms.clear();
    // render the suite aka player
    {
        glm::mat4 suiteModel = glm::mat4(1.0f);
        suiteModel = glm::translate(suiteModel, glm::vec3(2, 0.f, 2));
        transforms.push_back(suiteModel);
        suite->draw(modelShader, transforms);
    }

    transforms.clear();
    // render the walls
    {
        auto walls = aMap->GetWalls();
        if (walls.size() > 0)
        {
            for (auto const & w : walls)
            {
                glm::mat4 modelTransform = glm::mat4(1.0f);
                modelTransform = glm::translate(modelTransform, glm::vec3(w->GetX(), 0.f, w->GetY()));
                transforms.push_back(modelTransform);
            }
            wall->draw(modelShader, transforms);
        }
    }

    transforms.clear();
    // render the bricks
    {
        auto bricks = aMap->GetBricks();
        if (bricks.size() > 0)
        {
            for (auto const & b : bricks)
            {
                glm::mat4 modelTransform = glm::mat4(1.0f);
                modelTransform = glm::translate(modelTransform, glm::vec3(b->GetX(), 0.f, b->GetY()));
                transforms.push_back(modelTransform);
            }
            brick->draw(modelShader, transforms);
        }
    }

    // render skybox
    view = glm::mat4(glm::mat3(mCamera.getViewMatrix()));
    skyboxShader->use();
    skyboxShader->setMat4("view", view);
    skyboxShader->setMat4("projection", projection);
    skybox->draw(skyboxShader);
}

Camera &Renderer::getCamera()
{
    return mCamera;
}