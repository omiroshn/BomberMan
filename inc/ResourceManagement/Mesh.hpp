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
    glm::vec3   Normal;
    glm::vec2   TexCoords;
    glm::vec3   Tangent;
    glm::vec3   Bitangent;
    glm::ivec4  BonesID;
    glm::vec4   Weighs;
};

class Mesh
{
public:
    Mesh(std::vector<Vertex> aVertices,
        std::vector<unsigned int> aIndices,
        std::vector<std::shared_ptr<Texture>> aTextures,
        std::map<std::string, unsigned int> bones,
        std::vector<glm::mat4> aOffsets,
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
    aiNodeAnim const*       findNodeAnimation(aiAnimation const* animation, std::string nodeName) const;

private:
    unsigned int mVBO, mEBO, mVAO, mIBO;
    std::vector<Vertex> mVertices;
    std::vector<unsigned int> mIndices;
    std::vector<std::shared_ptr<Texture>> mTextures;
    bool                mIsAnimated;

    unsigned int                        mCurrentAnimation;
    float                               mAnimationTime;
    float                               mGlossiness;

    std::map<std::string, unsigned int> const   mBones;
    std::vector<glm::mat4>              const   mOffsetMatrices;
    std::vector<glm::mat4>                      mBoneTransforms;

    aiScene const*  mScene;
};
#endif