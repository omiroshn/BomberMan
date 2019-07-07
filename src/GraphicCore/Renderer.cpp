#include "GraphicCore/Renderer.hpp"
#include "Game.hpp"
#include "LogicCore/MapForRendering.h"
#include "ResourceManagement/Texture.hpp"
#include "ResourceManagement/Shader.hpp"
#include "ResourceManagement/Model.hpp"
#include "ResourceManagement/Skybox.hpp"
#include "imgui.h"
#include <iostream>
Renderer::Renderer() : mCamera(glm::vec3(0.0f, 10.0f, -3.0f))
{
	mParticleManager = std::make_unique<ParticleManager>();
};

Renderer::~Renderer()
{
};

void Renderer::updateSize(int aWidth, int aHeight)
{
    mWidth = aWidth;
    mHeight = aHeight;
}

void Renderer::draw(MapForRendering& aMap)
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    normalPass(aMap);
}

void Renderer::normalPass(MapForRendering& aMap)
{
    glViewport(0, 0, mWidth, mHeight);
    
    int stage = CONFIGURATION.getChosenStage();
    static std::shared_ptr<Model> bricks[] = {
        RESOURCES.getModel("unbreakableWall"),
        RESOURCES.getModel("breakableWall"),
        RESOURCES.getModel("breakableWall"),
        RESOURCES.getModel("breakableWall")
    };
    static std::shared_ptr<Skybox> skyboxes[] = {
        RESOURCES.getSkybox("defaultSkybox"),
        RESOURCES.getSkybox("lightblue"),
        RESOURCES.getSkybox("defaultSkybox"),
        RESOURCES.getSkybox("blue")
    };

    static auto heroModel = RESOURCES.getModel("hero");


    static auto perimeterWall = RESOURCES.getModel("perimeterWall");
    static auto unbreakableWall = RESOURCES.getModel("unbreakableWall");

    static auto balloon = RESOURCES.getModel("balloon");
    static auto ground = RESOURCES.getModel("ground");
    static auto modelShader = RESOURCES.getShader("modelShader");
    static auto skyboxShader = RESOURCES.getShader("skybox");

    auto brick = bricks[stage];
    auto skybox = skyboxes[stage];

    modelShader->use();
    glm::mat4 projection = glm::perspective(glm::radians(mCamera.zoom()), static_cast<float>(mWidth) / static_cast<float>(mHeight), 0.1f, 90.0f);
    glm::mat4 view = mCamera.getViewMatrix();

    modelShader->setMat4("projection", projection);
    modelShader->setMat4("view", view);
    modelShader->setVec3("lightPos", mCamera.position());

    std::vector<glm::mat4> transforms;

    {
        auto& Hero = aMap.GetHero();
        Hero.debug();
        transforms.push_back(Hero.getModelMatrix());
        heroModel->setAnimation(Hero.getAnimation());
        heroModel->draw(modelShader, transforms);
    }
    transforms.clear();

    // render balloons aka enemies
    {
		auto& Enemies = aMap.GetEnemies();
		for (auto It : Enemies)
			transforms.push_back(It->getModelMatrix());
		balloon->draw(modelShader, transforms);
    }
    transforms.clear();

    // render the walls
    {
        auto walls = aMap.GetWalls();
        if (!walls.empty())
        {
            for (auto w : walls)
            {
                glm::mat4 modelTransform = glm::mat4(1.0f);
                modelTransform = glm::translate(modelTransform, glm::vec3(w->getX() + 0.5, 0.f, w->getY() + 0.5));
                transforms.push_back(modelTransform);
            }
            unbreakableWall->draw(modelShader, transforms);
        }
    }

    transforms.clear();
    // render the bricks
    {
        auto bricks = aMap.GetBricks();
        if (!bricks.empty())
        {
            for (auto b : bricks)
            {
                glm::mat4 modelTransform = glm::mat4(1.0f);
                modelTransform = glm::translate(modelTransform, glm::vec3(b->getX() + 0.5, 0.f, b->getY() + 0.5));
                transforms.push_back(modelTransform);
            }
            brick->draw(modelShader, transforms);
        }
    }

    // render the ground
    {
        glm::mat4 groundModel = glm::mat4(1.0f);
        groundModel = glm::translate(groundModel, glm::vec3(.0f, -0.5f, .0f));
        groundModel = glm::scale(groundModel, glm::vec3(1.0f, 0.1f, 1.0f));

        CollisionInfo &info = Game::getCollisionInfo();
        for (int i = 0; i < info.Squares.size(); i++)
        {
            glm::mat4 groundTransform = groundModel;
            groundTransform = glm::translate(groundTransform,
                glm::vec3(i%info.width + .5f, 0, i/info.width + .5f)
            );
            transforms.push_back(groundTransform);
        }
        ground->draw(modelShader, transforms);
    }
    transforms.clear();

	// render running particle system
	try {
		mParticleManager->draw(projection, view);
	} catch (CustomException &ex) {
		std::cout << ex.what() << std::endl;
		exit(42);
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

ParticleManager *Renderer::getParticleManager()
{
	return mParticleManager.get();
}
