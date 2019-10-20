#ifndef MESH_H
#define MESH_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <memory>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/scene.h>

#include "ResourceManagement/Shader.hpp"
#include "ResourceManagement/Texture.hpp"
#include "ResourceManagement/Animation.h"
#include "Utilities/AABB.hpp"

struct Vertex
{
    glm::vec3   Position;
    GLint       Normal;
    GLint       Tangent;
    int16_t     TexCoords[2];
};

struct WeightData
{
    uint8_t		Weighs[3];
	uint8_t		BonesID[3];
};

class Mesh
{
public:
    Mesh();
    Mesh& operator=(const Mesh&) = default;
    Mesh(std::vector<Vertex>& aVertices,
        std::vector<WeightData>& aWeights,
        std::vector<unsigned int>& aIndices,
        std::vector<std::shared_ptr<Texture>>& aTextures,
        std::map<std::string, unsigned int>& bones,
        std::vector<glm::mat4>& aOffsets,
        aiScene const *scene,
		float glossiness);
    void                    draw(std::shared_ptr<Shader> const& shader, std::vector<glm::mat4> const& transforms, glm::mat4 const & parentTransform);
    void                    doAnimation();
    void	                setAnimation(Animation const& anim);
    ~Mesh();
private:
    void                    setupMesh();
    void                    setInstanceBuffer();
    void	                readNodeHierarchy(float animationTime, aiNode const* node, glm::mat4 parentTransform);
    aiNodeAnim const*       findNodeAnimation(aiAnimation const* animation, const std::string& nodeName) const;

private:
    unsigned int mVBO = 0, mEBO = 0, mVAO = 0, mWBO = 0, mIBO = 0;
    std::vector<Vertex> mVertices;
    std::vector<WeightData> mWeights;
    std::vector<unsigned int> mIndices;
    std::vector<std::shared_ptr<Texture>> mTextures;
    bool mIsAnimated = false;

    void setupAnimations();

    float mAnimationTime = 0.f;
    float mGlossiness = 1.f;
    AnimationType mCurrentAnimation = AnimationType::Idle;
    std::map<AnimationType, const aiAnimation *> mAnimations;
    std::map<std::string, unsigned int> const mBones;
    std::vector<glm::mat4> const mOffsetMatrices;
    std::vector<glm::mat4> mBoneTransforms;
    aiScene const *mScene = nullptr;
};
#endif