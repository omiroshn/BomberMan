#include "GraphicCore/Renderer.hpp"
#include "Game.hpp"
#include "ResourceManagement/Texture.hpp"
#include "ResourceManagement/Shader.hpp"
#include "ResourceManagement/Model.hpp"
#include "ResourceManagement/Skybox.hpp"
#include "imgui.h"
#include <iostream>
#include "Entity/MovingEntity.h"

Renderer::Renderer() :
    mCamera(glm::vec3(0.0f, 10.0f, -3.0f)),
    mParticleManager(std::make_unique<ParticleManager>()),
    mLightManager(std::make_unique<LightManager>()),
    mStage(CONFIGURATION.getChosenStage())
{
	//glGenVertexArrays(1, &mQuadsArray);
	//glGenBuffers(1, &mQuadsBuffer);
	//glBindVertexArray(mQuadsArray);
	//glBindBuffer(GL_ARRAY_BUFFER, mQuadsBuffer);

    //glEnableVertexAttribArray(0);	
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vert), (void*)0);
    //glEnableVertexAttribArray(1);	
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vert), (void *)offsetof(Vert, uv));
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
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    prepareTransforms(aMap);
    shadowPass(aMap);
    normalPass(aMap);
}

void Renderer::prepareTransforms(Game &g)
{
    mTransforms.clear();
    std::vector<glm::mat4> transforms;
    mTransforms[ModelType::Brick] = g.GetBrickTransforms();
    mTransforms[ModelType::Wall] = g.GetWallTransforms();
    mTransforms[ModelType::Player] = std::vector<glm::mat4>{g.GetHero().getModelMatrix()};
    auto& Enemies = g.GetEnemies();
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
    static auto perimeterWall = RESOURCES.getModel("perimeterWall");
    static auto unbreakableWall = RESOURCES.getModel("unbreakableWall");
    auto brick = bricks[mStage];

    unbreakableWall->draw(s, mTransforms[ModelType::Wall]);
    brick->draw(s, mTransforms[ModelType::Brick]);
}

void Renderer::renderMovable(std::shared_ptr<Shader> &s, Game &g)
{
    static auto heroModel = RESOURCES.getModel("hero");
    static auto balloon = RESOURCES.getModel("balloon");

    //render hero
    auto& Hero = g.GetHero();
    Hero.debug();
    heroModel->setAnimation(Hero.getAnimation());
    heroModel->draw(s, mTransforms[ModelType::Player]);
    //render enemies
    balloon->draw(s, mTransforms[ModelType::EnemyType1]);
}

void Renderer::normalPass(Game& aMap)
{
    glViewport(0, 0, mWidth, mHeight);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 
    static std::shared_ptr<Skybox> skyboxes[] =
    {
        RESOURCES.getSkybox("defaultSkybox"),
        RESOURCES.getSkybox("lightblue"),
        RESOURCES.getSkybox("defaultSkybox"),
        RESOURCES.getSkybox("blue")
    };

    static auto ground = RESOURCES.getModel("ground");
    static auto modelShader = RESOURCES.getShader("modelShader");
    static auto skyboxShader = RESOURCES.getShader("skybox");

    auto skybox = skyboxes[mStage];

    modelShader->use();
    glm::mat4 projection = glm::perspective(glm::radians(mCamera.zoom()), static_cast<float>(mWidth) / static_cast<float>(mHeight), 0.1f, 90.0f);
    glm::mat4 view = mCamera.getViewMatrix();

    modelShader->setMat4("projection", projection);
    modelShader->setMat4("view", view);
    modelShader->setVec3("viewPos", mCamera.position());
    modelShader->setVec3("lightPos", mLightManager->getCurrentLightPos());
    modelShader->setInt("shadowMap", mLightManager->bindDepthMap());
    modelShader->setMat4("lightSpaceMatrix", mLightManager->getLightSpaceMatrix());

    renderMovable(modelShader, aMap);
    renderObstacles(modelShader);

    // render the ground
    std::vector<glm::mat4> transforms;
    static float fake_timer = 0;
    // render bombs
    {
        //Animation a;
        //a.setTime(fake_timer);
        auto bombTransforms = aMap.GetBombTransforms();
        //bomb->setAnimation(a);
		bomb->draw(modelShader, bombTransforms);
        //fake_timer += 0.01f;
    }

    {
        Animation a;
        a.setTime(fake_timer);
        auto brickTransforms = aMap.GetBrickTransforms();
        brick->setAnimation(a);
		brick->draw(modelShader, brickTransforms);
        fake_timer += 0.01f;
    }

    {
	    auto& Hero = aMap.GetHero();
	    Hero.debug();

	    transforms.push_back(Hero.getModelMatrix());
	    heroModel->setAnimation(Hero.getAnimation());
	    heroModel->draw(modelShader, transforms);
	    glm::vec3 heroPosition = Hero.getPosition3D();
	    drawShadow(heroPosition);
    }
    transforms.clear();

    // render balloons aka enemies
    {
		auto& Enemies = aMap.GetEnemies();
		for (auto It : Enemies)
		{
			transforms.push_back(It->getModelMatrix());
			drawShadow(It->getPosition3D());
		}
		balloon->draw(modelShader, transforms);
    }
    transforms.clear();

    // render the walls
    {
        auto wallTransforms = aMap.GetWallTransforms();
		unbreakableWall->draw(modelShader, wallTransforms);
    }

    CollisionInfo &info = Game::getCollisionInfo();
    for (int i = 0; i < info.Squares.size(); i++)
    {
        glm::mat4 groundTransform = groundModel;
        groundTransform = glm::translate(groundTransform,
            glm::vec3(i % info.width + .5f, 0, i / info.width + .5f)
        );
        transforms.push_back(groundTransform);
    }
    ground->draw(modelShader, transforms);
	// render running particle system
	try
    {
		mParticleManager->draw(projection, view);
	}
    catch (CustomException &ex)
    {
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

void Renderer::shadowPass(Game& aMap)
{
    static auto shadowShader = RESOURCES.getShader("shadow");
    shadowShader->use();
    shadowShader->setMat4("lightSpaceMatrix", mLightManager->getLightSpaceMatrix());
    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glCullFace(GL_FRONT);
    glBindFramebuffer(GL_FRAMEBUFFER, mLightManager->getDepthFrameBufferID());
    glClear(GL_DEPTH_BUFFER_BIT);
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

//void Renderer::drawQuad(Quad quad)
//{
//	mShadowQuads.push_back(quad);
//}

//void Renderer::drawShadow(glm::vec3 position)
//{
//	float shadowYOffset = -0.445;
//	float shadowSize = 0.241f;
//	Renderer::Quad shadow(
//	    position + glm::vec3{ shadowSize,shadowYOffset, shadowSize},
//	    position + glm::vec3{ shadowSize,shadowYOffset,-shadowSize},
//	    position + glm::vec3{-shadowSize,shadowYOffset, shadowSize},
//	    position + glm::vec3{-shadowSize,shadowYOffset,-shadowSize}
//	);
//	drawQuad(shadow);
//}

//void Renderer::drawQuadsDeferred(glm::mat4 view, glm::mat4 projection)
//{
//	static auto ShadowShader = RESOURCES.getShader("shadow");
//	if (mShadowQuads.empty())
//		return;
//
//	glBindBuffer(GL_ARRAY_BUFFER, mQuadsBuffer);
//	glBufferData(GL_ARRAY_BUFFER, mShadowQuads.size() * sizeof(Renderer::Quad), mShadowQuads.data(), GL_DYNAMIC_DRAW);
//	glBindVertexArray(mQuadsArray);
//
//	ShadowShader->use();
//	ShadowShader->setMat4("view", view);
//	ShadowShader->setMat4("projection", projection);
//
//	glEnable(GL_BLEND);
//	glDepthMask(false);
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//
//	glDrawArrays(GL_TRIANGLES, 0, mShadowQuads.size() * 6);
//	mShadowQuads.clear();
//	glDisable(GL_BLEND);
//	glDepthMask(true);
//}