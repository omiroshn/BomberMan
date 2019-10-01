#include "GraphicCore/LightManager.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>
#include "Game.hpp"

const unsigned int SHADOW_SIZE = 1024, SHADOW_WIDTH = SHADOW_SIZE, SHADOW_HEIGHT = SHADOW_SIZE;

LightManager::LightManager()
{
    initShadowFramebuffer();
    //initLightSpaceMatrix();
}

LightManager::~LightManager()
{
}

void LightManager::prepareForShadowPass()
{
    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glCullFace(GL_FRONT);
    glBindFramebuffer(GL_FRAMEBUFFER, mDepthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);
}

void LightManager::initShadowFramebuffer()
{
    glGenFramebuffers(1, &mDepthMapFBO);
    mDepthTexture = std::make_unique<Texture>(TextureType::Depth, SHADOW_WIDTH, SHADOW_HEIGHT, nullptr, GL_DEPTH_COMPONENT, GL_LINEAR, GL_LINEAR);
    mDepthTexture->bind();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_GREATER);
    float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    glBindFramebuffer(GL_FRAMEBUFFER, mDepthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mDepthTexture->getTextureID(), 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void LightManager::initLightSpaceMatrix(glm::vec3 shadowMapOffset)
{
	static float angle = 1.979f;
    static glm::vec2 lr{-12.482f, 11.941f};
    static glm::vec2 tb{-10.824f, 7.2f};
    static glm::vec2 nf{38.294f, 71.647f};
    static glm::vec3 lookTarget(-1);
#if DEBUG
	ImGui::Begin("Lights");
	ImGui::SliderFloat("lightmap angle", &angle, 0.f, (float)M_PI * 2.f);
    ImGui::SliderFloat2("light left/right", &lr.x, -32, 32);
    ImGui::SliderFloat2("light top/bottom", &tb.x, -32, 32);
    ImGui::SliderFloat2("light near/far", &nf.x, 0, 120);
    ImGui::SliderFloat3("lookAtTarget", &lookTarget.x, 0, 100);
    ImGui::SliderFloat3("lightPos", &mLightPos.x, 0, 100);
	ImGui::Image(mDepthTexture.get(), ImVec2(400,400));
	ImGui::End();
#endif

    mLightDir = glm::normalize(mLightPos - lookTarget);
	glm::vec3 up = glm::vec3(0.0, 1.0, 0.0) * glm::mat3(glm::rotate(glm::mat4(1), angle, mLightDir));
    glm::mat4 lightView = glm::lookAt(mLightPos + shadowMapOffset, lookTarget + shadowMapOffset, up);
	glm::mat4 lightProjection = glm::ortho(lr.x, lr.y, tb.x, tb.y, nf.x, nf.y);
    mLightSpaceMatrix = lightProjection * lightView;
}

glm::mat4 const& LightManager::getLightSpaceMatrix()
{
    return mLightSpaceMatrix;
}

glm::vec3 const& LightManager::getCurrentLightDir() const
{
    return mLightDir;
}

int LightManager::bindDepthMap()
{
    glActiveTexture(GL_TEXTURE10);
    mDepthTexture->bind();
    return mDepthMapIndex;
}

GLuint LightManager::getDepthFrameBufferID()
{
    return mDepthMapFBO;
}