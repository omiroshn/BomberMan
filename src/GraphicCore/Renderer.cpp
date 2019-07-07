#include "GraphicCore/Renderer.hpp"
#include "Game.hpp"
#include "ResourceManagement/Texture.hpp"
#include "ResourceManagement/Shader.hpp"
#include "ResourceManagement/Model.hpp"
#include "ResourceManagement/Skybox.hpp"
#include "imgui.h"
#include <iostream>
#include "Entity/MovingEntity.h"

Renderer::Renderer() : mCamera(glm::vec3(0.0f, 10.0f, -3.0f))
{
	mParticleManager = std::make_unique<ParticleManager>();

	glGenVertexArrays(1, &mQuadsArray);
	glGenBuffers(1, &mQuadsBuffer);
	glBindVertexArray(mQuadsArray);
	glBindBuffer(GL_ARRAY_BUFFER, mQuadsBuffer);

    glEnableVertexAttribArray(0);	
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vert), (void*)0);
    glEnableVertexAttribArray(1);	
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vert), (void *)offsetof(Vert, uv));
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

    normalPass(aMap);
}

void Renderer::drawQuad(Quad quad)
{
	mShadowQuads.push_back(quad);
}

void Renderer::drawPicture(const std::string& pic)
{
    auto block = RESOURCES.getTexture("block");
    block->bind();
}

void Renderer::normalPass(Game& aMap)
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

    // render the bricks
    {
        auto brickTransforms = aMap.GetBrickTransforms();
        brick->draw(modelShader, brickTransforms);
    }

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

    drawQuadsDeferred(view, projection);

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

void Renderer::drawShadow(glm::vec3 position)
{
	float shadowYOffset = -0.445;
	float shadowSize = 0.241f;
	Renderer::Quad shadow(
	    position + glm::vec3{ shadowSize,shadowYOffset, shadowSize},
	    position + glm::vec3{ shadowSize,shadowYOffset,-shadowSize},
	    position + glm::vec3{-shadowSize,shadowYOffset, shadowSize},
	    position + glm::vec3{-shadowSize,shadowYOffset,-shadowSize}
	);
	drawQuad(shadow);
}

void Renderer::drawQuadsDeferred(glm::mat4 view, glm::mat4 projection)
{
	static auto ShadowShader = RESOURCES.getShader("shadow");
	if (mShadowQuads.empty())
		return;

	glBindBuffer(GL_ARRAY_BUFFER, mQuadsBuffer);
	glBufferData(GL_ARRAY_BUFFER, mShadowQuads.size() * sizeof(Renderer::Quad), mShadowQuads.data(), GL_DYNAMIC_DRAW);
	glBindVertexArray(mQuadsArray);

	ShadowShader->use();
	ShadowShader->setMat4("view", view);
	ShadowShader->setMat4("projection", projection);

	glEnable(GL_BLEND);
	glDepthMask(false);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDrawArrays(GL_TRIANGLES, 0, mShadowQuads.size() * 6);
	mShadowQuads.clear();
	glDisable(GL_BLEND);
	glDepthMask(true);
}

Camera &Renderer::getCamera()
{
    return mCamera;
}

ParticleManager *Renderer::getParticleManager()
{
	return mParticleManager.get();
}
