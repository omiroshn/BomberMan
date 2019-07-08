#include "GraphicCore/Renderer.hpp"
#include "Game.hpp"
#include "ResourceManagement/Texture.hpp"
#include "ResourceManagement/Shader.hpp"
#include "ResourceManagement/Model.hpp"
#include "ResourceManagement/Skybox.hpp"
#include "imgui.h"
#include <iostream>
#include "Entity/MovingEntity.h"
    const unsigned int SHADOW_WIDTH = 2028, SHADOW_HEIGHT = 2048;
    unsigned int depthMapFBO;
    unsigned int depthMap;
     //glm::vec3 lightPos(-20.0f, 40.0f, -10.0f);
     glm::vec3 lightPos(-20.0f, 50.0f, 0.0f);

Renderer::Renderer() : mCamera(glm::vec3(0.0f, 10.0f, -3.0f))
{
	mParticleManager = std::make_unique<ParticleManager>();

	//glGenVertexArrays(1, &mQuadsArray);
	//glGenBuffers(1, &mQuadsBuffer);
	//glBindVertexArray(mQuadsArray);
	//glBindBuffer(GL_ARRAY_BUFFER, mQuadsBuffer);

 //   glEnableVertexAttribArray(0);	
 //   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vert), (void*)0);
 //   glEnableVertexAttribArray(1);	
 //   glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vert), (void *)offsetof(Vert, uv));
// configure depth map FBO
    // -----------------------
    glGenFramebuffers(1, &depthMapFBO);
    // create depth texture
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    // attach depth texture as FBO's depth buffer
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
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

    shadowPass(aMap);
    normalPass(aMap);
}

//void Renderer::drawQuad(Quad quad)
//{
//	mShadowQuads.push_back(quad);
//}

void Renderer::normalPass(Game& aMap)
{
    glViewport(0, 0, mWidth, mHeight);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
    static auto bomb = RESOURCES.getModel("bomb");

    static auto balloon = RESOURCES.getModel("balloon");
    static auto ground = RESOURCES.getModel("ground");
    static auto modelShader = RESOURCES.getShader("modelShader");
    static auto skyboxShader = RESOURCES.getShader("skybox");

    auto brick = bricks[stage];
    auto skybox = skyboxes[stage];

    modelShader->use();
    glm::mat4 projection = glm::perspective(glm::radians(mCamera.zoom()), static_cast<float>(mWidth) / static_cast<float>(mHeight), 0.1f, 90.0f);
    glm::mat4 view = mCamera.getViewMatrix();

    glm::mat4 lightProjection, lightView;
    glm::mat4 lightSpaceMatrix;
    float near_plane = 1.0f, far_plane = 100.0f;
    //lightProjection = glm::perspective(glm::radians(45.0f), (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT, near_plane, far_plane); // note that if you use a perspective projection matrix you'll have to change the light position as the current light position isn't enough to reflect the whole scene
    lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
    auto& Hero = aMap.GetHero();
    lightView = glm::lookAt(lightPos, Hero.getPosition3D(), glm::vec3(0.0, 1.0, 0.0));
    lightSpaceMatrix = lightProjection * lightView;

    modelShader->setMat4("projection", projection);
    modelShader->setMat4("view", view);
    modelShader->setVec3("viewPos", mCamera.position());
    modelShader->setVec3("lightPos", lightPos);
    modelShader->setInt("shadowMap", 10);
    modelShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);
    glActiveTexture(GL_TEXTURE10);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    std::vector<glm::mat4> transforms;
    {
	    auto& Hero = aMap.GetHero();
	    Hero.debug();

	    transforms.push_back(Hero.getModelMatrix());
	    heroModel->setAnimation(Hero.getAnimation());
	    heroModel->draw(modelShader, transforms);
	    glm::vec3 heroPosition = Hero.getPosition3D();
	    //drawShadow(heroPosition);
    }
    transforms.clear();

    // render balloons aka enemies
    {
		auto& Enemies = aMap.GetEnemies();
		for (auto It : Enemies)
		{
			transforms.push_back(It->getModelMatrix());
			//drawShadow(It->getPosition3D());
		}
		balloon->draw(modelShader, transforms);
    }
    transforms.clear();

    // render the walls
    {
        auto wallTransforms = aMap.GetWallTransforms();
		unbreakableWall->draw(modelShader, wallTransforms);
    }

    //// render the bricks
    //{
    //    auto brickTransforms = aMap.GetBrickTransforms();
    //    brick->draw(modelShader, brickTransforms);
    //}

    // render the ground
    {
        glm::mat4 groundModel = glm::mat4(1.0f);
        groundModel = glm::translate(groundModel, glm::vec3(.0f, -1.f, .0f));

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

    //drawQuadsDeferred(view, projection);

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

void Renderer::shadowPass(Game& aMap)
{
        auto heroModel = RESOURCES.getModel("hero");
        auto perimeterWall = RESOURCES.getModel("perimeterWall");
        auto unbreakableWall = RESOURCES.getModel("unbreakableWall");
        auto balloon = RESOURCES.getModel("balloon");
        auto ground = RESOURCES.getModel("ground");
        auto shadowShader = RESOURCES.getShader("shadow");
        auto brick = RESOURCES.getModel("breakableWall");

        glm::mat4 lightProjection, lightView;
        glm::mat4 lightSpaceMatrix;
        float near_plane = 1.0f, far_plane = 100.0f;
        //lightProjection = glm::perspective(glm::radians(45.0f), (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT, near_plane, far_plane); // note that if you use a perspective projection matrix you'll have to change the light position as the current light position isn't enough to reflect the whole scene
        lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
        auto& Hero = aMap.GetHero();
        lightView = glm::lookAt(lightPos, Hero.getPosition3D(), glm::vec3(0.0, 1.0, 0.0));
        lightSpaceMatrix = lightProjection * lightView;

        // render scene from light's point of view
        shadowShader->use();
        shadowShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);

        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
            glClear(GL_DEPTH_BUFFER_BIT);
            glActiveTexture(GL_TEXTURE0);
             std::vector<glm::mat4> transforms;
    glCullFace(GL_FRONT);
    {
	    auto& Hero = aMap.GetHero();
	    Hero.debug();

	    transforms.push_back(Hero.getModelMatrix());
	    heroModel->setAnimation(Hero.getAnimation());
	    heroModel->draw(shadowShader, transforms);
	    glm::vec3 heroPosition = Hero.getPosition3D();
	    //drawShadow(heroPosition);
    }
    transforms.clear();

    // render balloons aka enemies
    {
		auto& Enemies = aMap.GetEnemies();
		for (auto It : Enemies)
		{
			transforms.push_back(It->getModelMatrix());
			//drawShadow(It->getPosition3D());
		}
		balloon->draw(shadowShader, transforms);
    }
    transforms.clear();

    // render the walls
    {
        auto wallTransforms = aMap.GetWallTransforms();
		unbreakableWall->draw(shadowShader, wallTransforms);
    }

    // render the bricks
    {
        auto brickTransforms = aMap.GetBrickTransforms();
        brick->draw(shadowShader, brickTransforms);
    }
    glCullFace(GL_BACK);

    //// render the ground
    //{
    //    glm::mat4 groundModel = glm::mat4(1.0f);
    //    groundModel = glm::translate(groundModel, glm::vec3(.0f, -1.f, .0f));

    //    CollisionInfo &info = Game::getCollisionInfo();
    //    for (int i = 0; i < info.Squares.size(); i++)
    //    {
    //        glm::mat4 groundTransform = groundModel;
    //        groundTransform = glm::translate(groundTransform,
    //            glm::vec3(i%info.width + .5f, 0, i/info.width + .5f)
    //        );
    //        transforms.push_back(groundTransform);
    //    }
    //    ground->draw(shadowShader, transforms);
    //}
    //transforms.clear();

    //glBindTexture(GL_TEXTURE_2D, woodTexture);
    //renderScene(simpleDepthShader);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //glCullFace(GL_BACK);
}

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

Camera &Renderer::getCamera()
{
    return mCamera;
}

ParticleManager *Renderer::getParticleManager()
{
	return mParticleManager.get();
}
