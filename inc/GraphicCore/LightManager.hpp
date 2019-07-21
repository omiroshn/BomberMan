#pragma once
#include <memory>
#include "GL/glew.h"
#include "glm/glm.hpp"

const unsigned int SHADOW_WIDTH = 2048, SHADOW_HEIGHT = 2048;
class LightManager
{
public:
    explicit LightManager();
    ~LightManager();

    glm::mat4 const& getLightSpaceMatrix() const;
    glm::vec3 const& getCurrentLightPos() const;
    int bindDepthMap();
    GLuint getDepthFrameBufferID();
private:
    int  mDepthMapIndex{10};
    void initShadowFramebuffer();
    void initLightSpaceMatrix();
    GLuint mDepthMapFBO;
    GLuint mDepthMap;
    glm::mat4 mLightSpaceMatrix;
    glm::vec3 mLightPos{-40.0f, 75.0f, 10.5};
};
typedef std::unique_ptr<LightManager> ptrLM;
