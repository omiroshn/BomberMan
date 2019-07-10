#include "GraphicCore/LightManager.hpp"
#include <glm/gtc/matrix_transform.hpp>

LightManager::LightManager()
{
    initShadowFramebuffer();
    initLightSpaceMatrix();
}

LightManager::~LightManager()
{
}

void LightManager::initShadowFramebuffer()
{
    glGenFramebuffers(1, &mDepthMapFBO);
    glGenTextures(1, &mDepthMap);
    glBindTexture(GL_TEXTURE_2D, mDepthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
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
    glm::mat4 lightProjection, lightView;
    lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.f, 100.0f);
    lightView = glm::lookAt(mLightPos, glm::vec3(11,0,9), glm::vec3(0.0, 1.0, 0.0));
    mLightSpaceMatrix = lightProjection * lightView;
}

glm::mat4 const& LightManager::getLightSpaceMatrix() const
{
    return mLightSpaceMatrix;
}

glm::vec3 const& LightManager::getCurrentLightPos() const
{
    return mLightPos;
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