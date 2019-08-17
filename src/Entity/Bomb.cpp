#include "Entity/Bomb.h"
#include "ResourceManagement/Shader.hpp"
#include "Game.hpp"
#include "imgui.h"

std::vector<glm::vec4>  Bomb::mSparksQuads;
GLuint Bomb::mSparksBuffer = 0;
GLuint Bomb::mSparksArray = 0;

const float Bomb::FUSE_TIME = 3.0f;
const float Bomb::SPAWN_TIME = .5f;

Bomb::Bomb(glm::vec2 pos, int explosionStrength)
	: Entity(glm::floor(pos) + glm::vec2{0.5f})
	, mExplosionStrength(explosionStrength)
{
	Game::getCollisionInfo()[getPosition()] = SquareType::Bomb;
}

void Bomb::bindArrays()
{
	glGenVertexArrays(1, &Bomb::mSparksArray);
    glGenBuffers(1, &Bomb::mSparksBuffer);
    glBindVertexArray(Bomb::mSparksArray);
    glBindBuffer(GL_ARRAY_BUFFER, Bomb::mSparksBuffer);

    glEnableVertexAttribArray(0); 
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);
}

Bomb::~Bomb()
{
	if (auto *hero = &Game::get()->getHero())
		hero->increaseBombCount();
	Game::getCollisionInfo()[getPosition()] = SquareType::EmptySquare;
	if (!mSparksQuads.empty())
		mSparksQuads.erase(mSparksQuads.begin());
}

void Bomb::drawSparks(glm::vec4 position)
{
	mSparksQuads.push_back(position);
}

void Bomb::drawSparksQuadsDeferred(glm::mat4 view, glm::mat4 projection)
{
    if (mSparksQuads.empty())
		return;
	static auto SparksShader = RESOURCES.getShader("sparks");
	
    static float offsetX = 0.0f;
    static float offsetY = 0.0f;

	offsetX += 1.0f / 8.0f;
	if (offsetX > 1.0f)
	{
		offsetX = 0.0f;
		offsetY += 1.0f / 8.0f;
		if (offsetY > 0.75f)
			offsetY = 0.25f;
	}
	
    glBindVertexArray(mSparksArray);
    glBindBuffer(GL_ARRAY_BUFFER, mSparksBuffer);
    glBufferData(GL_ARRAY_BUFFER, mSparksQuads.size() * sizeof(glm::vec4), mSparksQuads.data(), GL_DYNAMIC_DRAW);

	SparksShader->use();
	SparksShader->setMat4("view", view);
	SparksShader->setMat4("projection", projection);
    SparksShader->setFloat("offsetX", offsetX);
    SparksShader->setFloat("offsetY", offsetY);

	glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_ONE, GL_ONE);

    glActiveTexture(GL_TEXTURE0);
    RESOURCES.getTexture("sparks")->bind();

	glDrawArrays(GL_POINTS, 0, (GLsizei)mSparksQuads.size());
	glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
}

// SPAWNING
bool Spawning::transition(const Counting&)
{
	return (Game::getCurrentTime() - mStartTime) >= Bomb::SPAWN_TIME;
}

void Spawning::onTick(Bomb& bomb, float)
{
	bomb.setScale((Game::getCurrentTime() - mStartTime) / Bomb::SPAWN_TIME);
}

void Spawning::onEntry(Bomb& bomb)
{
	mStartTime = Game::getCurrentTime();
	bomb.setScale(0.f);
}

// COUNTING
void Counting::onTick(Bomb& bomb, float)
{
	if (Game::getCurrentTime() >= mTimeToExplode)
	{
		Game::get()->explosion(bomb.getPosition(), bomb.mExplosionStrength);
		bomb.kill();
	}
}

void Counting::onEntry(Bomb& bomb)
{
	bomb.setScale(1.f);
	mTimeToExplode = Game::getCurrentTime() + Bomb::FUSE_TIME - Bomb::SPAWN_TIME;
	mCountdown = mTimeToExplode - Game::getCurrentTime();
	glm::vec3 SparkPos = bomb.getPosition3D() + glm::vec3({-0.1f, 0.5f, 0.f});
	bomb.drawSparks(glm::vec4(SparkPos, mCountdown));
}

