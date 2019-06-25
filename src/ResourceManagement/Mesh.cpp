   
#include "ResourceManagement/Mesh.hpp"

namespace
{
    unsigned int findPosition(float animationTime, const aiNodeAnim *node) {
        for (unsigned int i = 0; i < node->mNumPositionKeys - 1; i++) {
            if (animationTime < static_cast<float>(node->mPositionKeys[i + 1].mTime))
                return i;
        }
        return 0;
    }

    unsigned int findRotation(float animationTime, const aiNodeAnim *node) {
        for (unsigned int i = 0; i < node->mNumRotationKeys - 1; i++) {
            if (animationTime < static_cast<float>(node->mRotationKeys[i + 1].mTime))
                return i;
        }
        return 0;
    }

    unsigned int findScale(float animationTime, const aiNodeAnim *node) {
        for (unsigned int i = 0; i < node->mNumScalingKeys - 1; i++) {
            if (animationTime < static_cast<float>(node->mScalingKeys[i + 1].mTime))
                return i;
        }
        return 0;
    }

    glm::mat4 quatToMat(const aiQuaternion &quat) {
        float yy2 = 2.0f * quat.y * quat.y;
        float xy2 = 2.0f * quat.x * quat.y;
        float xz2 = 2.0f * quat.x * quat.z;
        float yz2 = 2.0f * quat.y * quat.z;
        float zz2 = 2.0f * quat.z * quat.z;
        float wz2 = 2.0f * quat.w * quat.z;
        float wy2 = 2.0f * quat.w * quat.y;
        float wx2 = 2.0f * quat.w * quat.x;
        float xx2 = 2.0f * quat.x * quat.x;

        glm::mat4 m;
        m[0][0] = - yy2 - zz2 + 1.0f;
        m[0][1] = xy2 + wz2;
        m[0][2] = xz2 - wy2;
        m[0][3] = 0;
        m[1][0] = xy2 - wz2;
        m[1][1] = - xx2 - zz2 + 1.0f;
        m[1][2] = yz2 + wx2;
        m[1][3] = 0;
        m[2][0] = xz2 + wy2;
        m[2][1] = yz2 - wx2;
        m[2][2] = - xx2 - yy2 + 1.0f;
        m[2][3] = 0.0f;
        m[3][0] = m[3][1] = m[3][2] = 0;
        m[3][3] = 1.0f;

        return m;
    }

    aiVector3D          calcInterpolatedScaling(double animationTime, const aiNodeAnim *node) {
        if(node->mNumScalingKeys == 1) {
            aiVector3D ret = node->mScalingKeys[0].mValue;
            return ret;
        }

        unsigned int scaleInd = findScale(animationTime, node);
        unsigned int nextScaleInd = scaleInd + 1;

        float deltaTime = static_cast<float>(node->mScalingKeys[nextScaleInd].mTime - node->mScalingKeys[scaleInd].mTime);
        float factor = (animationTime - static_cast<float>(node->mScalingKeys[scaleInd].mTime)) / deltaTime;
        aiVector3D start = node->mScalingKeys[scaleInd].mValue;
        aiVector3D end = node->mScalingKeys[nextScaleInd].mValue;

        aiVector3D delta = end - start;

        return start + factor * delta;
    }

    glm::mat4        calcInterpolatedRotation(double animationTime, const aiNodeAnim *node) {
        if (node->mNumRotationKeys == 1)
        {
            aiQuaternion ret = node->mRotationKeys[0].mValue;
            return quatToMat(ret);
        }
        unsigned int rotInd = findRotation(animationTime, node);
        unsigned int nextRotInd = rotInd + 1;

        float deltaTime = static_cast<float>(node->mRotationKeys[nextRotInd].mTime - node->mRotationKeys[rotInd].mTime);
        float factor = (animationTime - static_cast<float>(node->mRotationKeys[rotInd].mTime)) / deltaTime;
        aiQuaternion start = node->mRotationKeys[rotInd].mValue;
        aiQuaternion end = node->mRotationKeys[nextRotInd].mValue;

        aiQuaternion quat;
        aiQuaternion::Interpolate(quat, start, end, factor);
        quat = quat.Normalize();

        return quatToMat(quat);
    }

    aiVector3D          calcInterpolatedPosition(float animationTime, const aiNodeAnim *node) {
        if(node->mNumPositionKeys == 1) {
            aiVector3D ret = node->mPositionKeys[0].mValue;
            return ret;
        }

        unsigned int posInd = findPosition(animationTime, node);
        unsigned int nextPosInd = posInd + 1;

        float deltaTime = static_cast<float>(node->mPositionKeys[nextPosInd].mTime - node->mPositionKeys[posInd].mTime);
        float factor = (animationTime - static_cast<float>(node->mPositionKeys[posInd].mTime)) / deltaTime;
        aiVector3D start = node->mPositionKeys[posInd].mValue;
        aiVector3D end = node->mPositionKeys[nextPosInd].mValue;

        aiVector3D delta = end - start;

        return start + factor * delta;
    }


    glm::mat4			aiMatToGlmMat(aiMatrix4x4 ai)
    {
        glm::mat4 mat;
        mat[0][0] = ai.a1; mat[1][0] = ai.a2; mat[2][0] = ai.a3; mat[3][0] = ai.a4;
        mat[0][1] = ai.b1; mat[1][1] = ai.b2; mat[2][1] = ai.b3; mat[3][1] = ai.b4;
        mat[0][2] = ai.c1; mat[1][2] = ai.c2; mat[2][2] = ai.c3; mat[3][2] = ai.c4;
        mat[0][3] = ai.d1; mat[1][3] = ai.d2; mat[2][3] = ai.d3; mat[3][3] = ai.d4;
        return mat;
    }
}

Mesh::Mesh(std::vector<Vertex> vertices,
            std::vector<unsigned int> indices,
            std::vector<std::shared_ptr<Texture>> textures,
            std::map<std::string, unsigned int> bones,
            std::vector<glm::mat4> aOffsets,
            aiScene const* scene) :
    mVertices{std::move(vertices)}
    , mIndices{std::move(indices)}
    , mTextures{std::move(textures)}
    , mCurrentAnimation{0}
    , mScene{scene}
    , mIsAnimated{scene->HasAnimations()}
    , mOffsetMatrices{std::move(aOffsets)}
    , mBones{std::move(bones)}
{
    setupMesh();
    setInstanceBuffer();
    if(mIsAnimated)
        mBoneTransforms.resize(mBones.size());
}

Mesh::~Mesh()
{
    glDeleteVertexArrays(1, &mVAO);
    glDeleteBuffers(1, &mVBO);
    glDeleteBuffers(1, &mEBO);
    glDeleteBuffers(1, &mIBO);
}

void Mesh::setupMesh()
{
    glGenVertexArrays(1, &mVAO);
    glGenBuffers(1, &mVBO);
    glGenBuffers(1, &mEBO);

    glBindVertexArray(mVAO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(Vertex), &mVertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(unsigned int), &mIndices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);	
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    glEnableVertexAttribArray(1);	
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

    glEnableVertexAttribArray(2);	
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));


    glEnableVertexAttribArray(7);
    glVertexAttribIPointer(7, 3, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, bonesID));

    glEnableVertexAttribArray(8);
    glVertexAttribPointer(8, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, weigths));

    glBindVertexArray(0);
}

void	Mesh::setInstanceBuffer()
{
    glBindVertexArray(mVAO);
    glGenBuffers(1, &mIBO);

    glBindBuffer(GL_ARRAY_BUFFER, mIBO);
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4) * 4, (GLvoid*)(0 * sizeof(glm::vec4)));
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4) * 4, (GLvoid*)(1 * sizeof(glm::vec4)));
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4) * 4, (GLvoid*)(2 * sizeof(glm::vec4)));
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4) * 4, (GLvoid*)(3 * sizeof(glm::vec4)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);
    glVertexAttribDivisor(6, 1);

    glBindVertexArray(0);
}

void Mesh::draw(std::shared_ptr<Shader> shader, std::vector<glm::mat4> const & transforms)
{
    unsigned int diffuseNr  = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr   = 1;
    unsigned int heightNr   = 1;

    for(unsigned int i = 0; i < mTextures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        std::string number;
        std::string name = mTextures[i]->getTextureType();
        if(name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if(name == "texture_specular")
            number = std::to_string(specularNr++);
        else if(name == "texture_normal")
            number = std::to_string(normalNr++);
        else if(name == "texture_height")
            number = std::to_string(heightNr++);

        glUniform1i(glGetUniformLocation(shader->mShaderProgram, (name + number).c_str()), i);
        glBindTexture(GL_TEXTURE_2D, mTextures[i]->getTextureID());
    }
    glActiveTexture(GL_TEXTURE0);

    glBindBuffer(GL_ARRAY_BUFFER, mIBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * transforms.size(), &transforms[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    shader->setBool("isAnimated", mIsAnimated);
    if(mIsAnimated)
    {
        for (unsigned int i = 0; i < mBoneTransforms.size(); ++i)
        {
            shader->setMat4("boneTransforms[" + std::to_string(i) + "]", mBoneTransforms[i]);
        }
    }
    glBindVertexArray(mVAO);
    glDrawElementsInstanced(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, nullptr, transforms.size());
    glBindVertexArray(0);
}

const aiNodeAnim *Mesh::findNodeAnimation(const aiAnimation *animation, const std::string nodeName) const
{
    for (unsigned int i = 0; i < animation->mNumChannels; i++)
    {
        const aiNodeAnim *nodeAnim = animation->mChannels[i];
        if (std::string(nodeAnim->mNodeName.data) == nodeName)
            return nodeAnim;
    }
    return nullptr;
}


void	Mesh::readNodeHierarchy(float animationTime, aiNode const* node, const glm::mat4 parentTransform)
{

    std::string nodeName(node->mName.data);
    glm::mat4 nodeTransform = aiMatToGlmMat(node->mTransformation);

    auto const* animation = mScene->mAnimations[mCurrentAnimation];
    auto const* pNodeAnimation = findNodeAnimation(animation, nodeName);

    if(pNodeAnimation)
    {
        aiVector3D scaling = calcInterpolatedScaling(animationTime, pNodeAnimation);
        glm::mat4 scaleMat = glm::mat4(1.f);
        scaleMat = glm::scale(scaleMat, glm::vec3(scaling.x, scaling.y, scaling.z));

        glm::mat4 rotMat = glm::mat4(1.f);
        rotMat = calcInterpolatedRotation(animationTime, pNodeAnimation);

        glm::mat4 transMat = glm::mat4(1.f);
        aiVector3D translation = calcInterpolatedPosition(animationTime, pNodeAnimation);
        transMat = glm::translate(transMat, glm::vec3(translation.x, translation.y, translation.z));

        nodeTransform = transMat * rotMat * scaleMat;
    }

    glm::mat4 globalTransform = parentTransform * nodeTransform;
    if (mBones.find(nodeName) != mBones.end())
    {
        unsigned int boneIndex = mBones.at(nodeName);
        mBoneTransforms[boneIndex] = globalTransform * mOffsetMatrices[boneIndex];
    }
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        readNodeHierarchy(animationTime, node->mChildren[i], globalTransform);
    }
}

void	Mesh::doAnimation()
{
    float ticksPerSecond = mScene->mAnimations[mCurrentAnimation]->mTicksPerSecond;
    float timeInTicks = mAnimationTime * ticksPerSecond;
    float animTime = fmodf(timeInTicks, mScene->mAnimations[mCurrentAnimation]->mDuration - 1);
    readNodeHierarchy(animTime, mScene->mRootNode, glm::mat4(1.0f));
}

void	Mesh::setAnimation(unsigned int animation, float timeInSeconds)
{
    mAnimationTime = timeInSeconds;
    mCurrentAnimation = animation;
}