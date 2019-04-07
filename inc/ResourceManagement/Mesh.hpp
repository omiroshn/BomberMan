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
#include "Utilities/AABB.hpp"

struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;

    glm::ivec3 bonesID;
    glm::vec3  weigths;
};

class Mesh
{
public:
    Mesh(std::vector<Vertex> aVertices, std::vector<unsigned int> aIndices, std::vector<std::shared_ptr<Texture>> aTextures, const aiScene *scene, const aiMesh *pMesh);
    void draw(std::shared_ptr<Shader> shader, std::vector<glm::mat4> const & transforms);
    ~Mesh();
    void                    doAnimation();
    void                    setAnimation(unsigned int animation, float timeInSeconds);
private:
    void                    setupMesh();
    void                    setInstanceBuffer();
    void                    setupBones();
    void                    addBoneData(unsigned int vertexID, unsigned int boneID, float weight);

    void	                readNodeHierarchy(double animationTime, const aiNode *node, glm::mat4 parentTransform);
    aiNodeAnim const*       findNodeAnim(const aiAnimation *animation, std::string nodeName) const;

private:
    unsigned int mVBO, mEBO, mVAO, mIBO;
    std::vector<Vertex> mVertices;
    std::vector<unsigned int> mIndices;
    std::vector<std::shared_ptr<Texture>> mTextures;

    unsigned int                        mCurrentAnimation;
    unsigned int                        mTotalBones;
    float                               mAnimationTime;

    std::map<std::string, unsigned int> mBones;
    std::vector<glm::mat4>              mOffsetMatrices;
    std::vector<glm::mat4>              mJointTransforms;

    const aiScene       *mScene;
    const aiMesh *pMesh;



};
#endif