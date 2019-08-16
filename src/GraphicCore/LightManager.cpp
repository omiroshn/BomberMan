#include "GraphicCore/LightManager.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>

const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

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
    glGenTextures(1, &mDepthMap);
    glBindTexture(GL_TEXTURE_2D, mDepthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_GREATER);
    float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    glBindFramebuffer(GL_FRAMEBUFFER, mDepthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mDepthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void LightManager::initLightSpaceMatrix()
{
    static glm::vec2 lr{-1.882f, 24.941f};
    static glm::vec2 tb{-2.824f, 14.688f};
    static glm::vec2 nf{50.294f, 77.647f};
    static glm::vec3 lookTarget(-1);
	ImGui::Begin("Lights");
    ImGui::SliderFloat2("light left/right", &lr.x, -32, 32);
    ImGui::SliderFloat2("light top/bottom", &tb.x, -32, 32);
    ImGui::SliderFloat2("light near/far", &nf.x, 0, 120);
    ImGui::SliderFloat3("lookAtTarget", &lookTarget.x, 0, 100);
    ImGui::SliderFloat3("lightPos", &mLightPos.x, 0, 100);
	ImGui::End();

    glm::mat4 lightProjection, lightView;
    lightProjection = glm::ortho(lr.x, lr.y, tb.x, tb.y, nf.x, nf.y);
    lightView = glm::lookAt(mLightPos, lookTarget, glm::vec3(0.0, 1.0, 0.0));
	mLightDir = glm::normalize(mLightPos - lookTarget);
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
    glBindTexture(GL_TEXTURE_2D, mDepthMap);
    return mDepthMapIndex;
}

GLuint LightManager::getDepthFrameBufferID()
{
    return mDepthMapFBO;
}