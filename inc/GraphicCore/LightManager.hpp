#pragma once
#include <memory>
#include "GL/glew.h"
#include "glm/glm.hpp"
#include "ResourceManagement/Texture.hpp"

class LightManager
{
public:
    explicit LightManager();
    ~LightManager();
	LightManager(LightManager const&) = delete;
    LightManager& operator=(LightManager const&) = delete;
    glm::mat4 const& getLightSpaceMatrix();
    glm::vec3 const& getCurrentLightDir() const;
    int bindDepthMap();
    GLuint getDepthFrameBufferID();
    void prepareForShadowPass();
	void initLightSpaceMatrix(glm::vec3 shadowMapOffset);
    std::shared_ptr<Texture> mDepthTexture;
private:
    int  mDepthMapIndex{10};
    void initShadowFramebuffer();
    GLuint mDepthMapFBO;
    glm::mat4 mLightSpaceMatrix;
    glm::vec3 mLightPos{-40.0f, 36.905f, 10.5};
	glm::vec3 mLightDir;
};
typedef std::unique_ptr<LightManager> ptrLM;
