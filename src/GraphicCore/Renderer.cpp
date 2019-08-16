#include "GraphicCore/Renderer.hpp"
#include "Game.hpp"
#include "ResourceManagement/Texture.hpp"
#include "ResourceManagement/Shader.hpp"
#include "ResourceManagement/Model.hpp"
#include "ResourceManagement/Skybox.hpp"
#include "imgui.h"
#include <iostream>
#include "Entity/MovingEntity.h"
#include "Configure.hpp"

Renderer::Renderer() :
    mCamera(glm::vec3(0.0f, 10.0f, -3.0f)),
    mParticleManager(std::make_unique<ParticleManager>()),
    mLightManager(std::make_unique<LightManager>()),
    mStage(CONFIGURATION.getChosenStage())
{
    Bomb::bindArrays();
};

Renderer::~Renderer()
{
};

void Renderer::updateSize(int aWidth, int aHeight)
{
    mWidth = aWidth;
    mHeight = aHeight;
}

void Renderer::draw(Game& aMap)
{
    //glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_CULL_FACE);
	mLightManager->initLightSpaceMatrix();
    prepareTransforms(aMap);
    shadowPass(aMap);
    normalPass(aMap);
}

void Renderer::prepareTransforms(Game &g)
{
    mTransforms[ModelType::Brick] = g.getBrickTransforms();
    mTransforms[ModelType::Wall] = g.getWallTransforms();
    mTransforms[ModelType::Bomb] = g.getBombTransforms();
    mTransforms[ModelType::Player] = std::vector<glm::mat4>{g.getHero().getModelMatrix()};

    std::vector<glm::mat4> transforms;
    auto& Enemies = g.getEnemies();
    for (auto It : Enemies)
        transforms.push_back(It->getModelMatrix());
    mTransforms[ModelType::EnemyType1] = transforms;
}

void Renderer::renderObstacles(std::shared_ptr<Shader> &s)
{
    static std::shared_ptr<Model> bricks[] =
    {
        RESOURCES.getModel("unbreakableWall"),
        RESOURCES.getModel("breakableWall"),
        RESOURCES.getModel("breakableWall"),
        RESOURCES.getModel("breakableWall")
    };
    auto brick = bricks[mStage];

    static auto perimeterWall = RESOURCES.getModel("perimeterWall");
    static auto unbreakableWall = RESOURCES.getModel("unbreakableWall");
    static auto bomb = RESOURCES.getModel("bomb");

    unbreakableWall->draw(s, mTransforms[ModelType::Wall]);
    brick->draw(s, mTransforms[ModelType::Brick]);
    Animation a;
    a.setTime(0);
    bomb->setAnimation(a);
    bomb->draw(s, mTransforms[ModelType::Bomb]);

    auto type = Game::get()->powerupTypeOnMap();
    if (type != Hero::PowerupType::PT_NONE)
    {
        static std::shared_ptr<Model> bonusModels[Hero::PowerupType::PT_NONE] = {
            RESOURCES.getModel("bonus_bombs"),
            RESOURCES.getModel("bonus_flames"),
            RESOURCES.getModel("bonus_speed"),
            RESOURCES.getModel("bonus_wallpass"),
            RESOURCES.getModel("bonus_detonator"),
            RESOURCES.getModel("bonus_bombpass"),
            RESOURCES.getModel("bonus_flamepass")
        };
        bonusModels[type]->draw(s, std::vector<glm::mat4>{Game::get()->getPowerupTransform()});
    }
}

void Renderer::renderMovable(std::shared_ptr<Shader> &s, Game &g)
{
    static auto heroModel = RESOURCES.getModel("hero");
    static auto balloon = RESOURCES.getModel("balloon");

    //render hero
    auto& Hero = g.getHero();
    heroModel->setAnimation(Hero.getAnimation());
    heroModel->draw(s, mTransforms[ModelType::Player]);

    //render enemies
    balloon->draw(s, mTransforms[ModelType::EnemyType1]);
}

void Renderer::normalPass(Game& aMap)
{
    glViewport(0, 0, mWidth, mHeight);
    glClear(GL_DEPTH_BUFFER_BIT);
 
    static auto ground = RESOURCES.getModel("ground");
    static auto modelShader = RESOURCES.getShader("modelShader");
    static auto skyboxShader = RESOURCES.getShader("skybox");

    glm::mat4 projection = glm::perspective(glm::radians(mCamera.zoom()), static_cast<float>(mWidth) / static_cast<float>(mHeight), 0.1f, 90.0f);
    glm::mat4 view = mCamera.getViewMatrix();


	static float Shininess = 32.f;
	static float Glossiness = 0.5f;

	ImGui::SliderFloat("Shininess", &Shininess, 1.f, 32.f);
	ImGui::SliderFloat("Glossiness", &Glossiness, 0.f, 1.f);

    //render the model
    modelShader->use();
    modelShader->setMat4("projection", projection);
    modelShader->setMat4("view", view);
    modelShader->setVec3("viewPos", mCamera.position());
    modelShader->setVec3("lightDir", mLightManager->getCurrentLightDir());
    modelShader->setInt("shadowMap", mLightManager->bindDepthMap());
    modelShader->setMat4("lightSpaceMatrix", mLightManager->getLightSpaceMatrix());
    modelShader->setFloat("shininess", Shininess);
    modelShader->setFloat("glossiness", Glossiness);

    renderMovable(modelShader, aMap);
    renderObstacles(modelShader);

    // render the ground
    std::vector<glm::mat4> transforms;
    glm::mat4 groundModel = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, -1.f, .0f));

    CollisionInfo &info = Game::getCollisionInfo();
    for (size_t i = 0; i < info.Squares.size(); i++)
    {
        glm::mat4 groundTransform = glm::translate(groundModel,
            glm::vec3(i % info.width + .5f, 0, i / info.width + .5f)
        );
        transforms.push_back(groundTransform);
    }
    ground->draw(modelShader, transforms);

    // render skybox
    static std::shared_ptr<Skybox> skyboxes[] =
    {
        RESOURCES.getSkybox("defaultSkybox"),
        RESOURCES.getSkybox("lightblue"),
        RESOURCES.getSkybox("defaultSkybox"),
        RESOURCES.getSkybox("blue")
    };
    auto skybox = skyboxes[mStage];

    view = glm::mat4(glm::mat3(mCamera.getViewMatrix()));
    skyboxShader->use();
    skyboxShader->setMat4("view", view);
    skyboxShader->setMat4("projection", projection);
    skybox->draw(skyboxShader);

    view = mCamera.getViewMatrix();
	// render running particle system
	try {
		mParticleManager->draw(projection, view);
	} catch (CustomException &ex) {
		std::cout << ex.what() << std::endl;
		exit(42);
	}

    // render sparks
    Bomb::drawSparksQuadsDeferred(view, projection);
}

void Renderer::shadowPass(Game& aMap)
{
    static auto shadowShader = RESOURCES.getShader("shadow");
    shadowShader->use();
    shadowShader->setMat4("lightSpaceMatrix", mLightManager->getLightSpaceMatrix());
    mLightManager->prepareForShadowPass();
    renderMovable(shadowShader, aMap);
    renderObstacles(shadowShader);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glCullFace(GL_BACK);
}

Camera &Renderer::getCamera()
{
    return mCamera;
}

ParticleManager *Renderer::getParticleManager()
{
	return mParticleManager.get();
}