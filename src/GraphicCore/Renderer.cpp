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
    mStage = CONFIGURATION.getChosenStage();
    glEnable(GL_CULL_FACE);
	mLightManager->initLightSpaceMatrix(aMap.getHero().getPosition3D());
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
    bomb->draw(s, mTransforms[ModelType::Bomb]);

    auto type = Game::get()->powerupTypeOnMap();
    if (type != Hero::PowerupType::PT_NONE)
    {
        static std::shared_ptr<Model> bonusModel = RESOURCES.getModel("bonus");
        bonusModel->draw(s, std::vector<glm::mat4>{Game::get()->getPowerupTransform()});
    }

	bool active = Game::get()->isExitActive();
	static std::shared_ptr<Model> exit[2] = {
		RESOURCES.getModel("gate1"),
		RESOURCES.getModel("gate2"),
	};
	exit[active]->draw(s, std::vector<glm::mat4>{Game::get()->getExitTransform()});
}

void Renderer::renderMovable(std::shared_ptr<Shader> &s, std::shared_ptr<Shader> &animated, Game &g)
{
    static auto heroModel = RESOURCES.getModel("hero");
    static auto balloon = RESOURCES.getModel("enemy2");
    static auto onil = RESOURCES.getModel("enemy1");
    static auto ovape = RESOURCES.getModel("enemy3");

    //render hero
    auto& Hero = g.getHero();
    heroModel->setAnimation(Hero.getAnimation());
    heroModel->draw(animated, mTransforms[ModelType::Player]);

    //render enemies
    for (MovingEntity *It : g.mBalloons)
    {
        if (!balloon->mAnimated)
            balloon->draw(s, {It->getModelMatrix()});
        else
        {
            balloon->setAnimation(It->getAnimation());
            balloon->draw(animated, {It->getModelMatrix()});
        }
    }
    for (MovingEntity *It : g.mOnils)
    {
        if (!onil->mAnimated)
            onil->draw(s, {It->getModelMatrix()});
        else
        {
            onil->setAnimation(It->getAnimation());
            onil->draw(animated, {It->getModelMatrix()});
        }
    }
    for (MovingEntity *It : g.mOvapes)
    {
        if (!ovape->mAnimated)
            ovape->draw(s, {It->getModelMatrix()});
        else
        {
            ovape->setAnimation(It->getAnimation());
            ovape->draw(animated, {It->getModelMatrix()});
        }
    }
}

void Renderer::normalPass(Game& aMap)
{
    glViewport(0, 0, mWidth, mHeight);
    glClear(GL_DEPTH_BUFFER_BIT);
    glCullFace(GL_BACK);
 
    static auto ground = RESOURCES.getModel("ground");
    static auto modelShader = RESOURCES.getShader("modelShader");
    static auto animatedModelShader = RESOURCES.getShader("animatedModelShader");
    static auto skyboxShader = RESOURCES.getShader("skybox");

    glm::mat4 projection = glm::perspective(glm::radians(mCamera.zoom()), static_cast<float>(mWidth) / static_cast<float>(mHeight), 0.1f, 90.0f);
    glm::mat4 view = mCamera.getViewMatrix();

	static float Shininess = 32.f;

#if DEBUG
	ImGui::SliderFloat("Shininess", &Shininess, 8.f, 64.f);
#endif

    //render the model
    modelShader->use();
    modelShader->setMat4("projection", projection);
    modelShader->setMat4("view", view);
    modelShader->setVec3("viewPos", mCamera.position());
    modelShader->setVec3("lightDir", mLightManager->getCurrentLightDir());
    modelShader->setInt("shadowMap", mLightManager->bindDepthMap());
    modelShader->setMat4("lightSpaceMatrix", mLightManager->getLightSpaceMatrix());
    modelShader->setFloat("shininess", Shininess);

    animatedModelShader->use();
    animatedModelShader->setMat4("projection", projection);
    animatedModelShader->setMat4("view", view);
    animatedModelShader->setVec3("viewPos", mCamera.position());
    animatedModelShader->setVec3("lightDir", mLightManager->getCurrentLightDir());
    animatedModelShader->setInt("shadowMap", mLightManager->bindDepthMap());
    animatedModelShader->setMat4("lightSpaceMatrix", mLightManager->getLightSpaceMatrix());
    animatedModelShader->setFloat("shininess", Shininess);

    renderMovable(modelShader,animatedModelShader, aMap);
    renderObstacles(modelShader);

    // render the ground
    glm::mat4 groundModel = glm::translate(glm::mat4(1.0f), glm::vec3(.0f, -1.f, .0f));

    CollisionInfo &info = Game::getCollisionInfo();
    std::vector<glm::mat4> transforms(info.Squares.size());
    for (size_t i = 0; i < info.Squares.size(); i++)
    {
        glm::mat4 groundTransform = glm::translate(groundModel,
            glm::vec3(i % info.width + .5f, 0, i / info.width + .5f)
        );
        transforms[i] = groundTransform;
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
	}

    // render sparks
    Bomb::drawSparksQuadsDeferred(view, projection);
}

void Renderer::shadowPass(Game& aMap)
{
    static auto shadowShader = RESOURCES.getShader("shadow");
    static auto animatedShadowShader = RESOURCES.getShader("animatedShadow");
    shadowShader->use();
    shadowShader->setMat4("lightSpaceMatrix", mLightManager->getLightSpaceMatrix());
    animatedShadowShader->use();
    animatedShadowShader->setMat4("lightSpaceMatrix", mLightManager->getLightSpaceMatrix());
    mLightManager->prepareForShadowPass();
    renderMovable(shadowShader, animatedShadowShader, aMap);
    renderObstacles(shadowShader);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Camera &Renderer::getCamera()
{
    return mCamera;
}

ParticleManager *Renderer::getParticleManager()
{
	return mParticleManager.get();
}