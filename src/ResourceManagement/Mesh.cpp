   
#include "ResourceManagement/Mesh.hpp"
#include "Utilities/AnimationUtils.h"

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
    glVertexAttribIPointer(7, 3, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, BonesID));

    glEnableVertexAttribArray(8);
    glVertexAttribPointer(8, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Weighs));

    glEnableVertexAttribArray(9);
    glVertexAttribPointer(9, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));

    glEnableVertexAttribArray(10);
    glVertexAttribPointer(10, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));

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

void Mesh::draw(std::shared_ptr<Shader> const &shader, std::vector<glm::mat4> const & transforms)
{
	if (transforms.size() == 0)
		return;
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
        else if (name == "texture_specular")
            number = std::to_string(specularNr++);
        else if (name == "texture_normal")
            number = std::to_string(normalNr++);
        else if (name == "texture_height")
            number = std::to_string(heightNr++);
        glUniform1i(glGetUniformLocation(shader->mShaderProgram, (name + number).c_str()), i);
        glBindTexture(GL_TEXTURE_2D, mTextures[i]->getTextureID());
    }
    shader->setBool("hasNormalMap", normalNr > 1);
    shader->setBool("hasHeightMap", heightNr > 1);
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
    glm::mat4 nodeTransform = AnimationUtils::aiMatToGlmMat(node->mTransformation);

    auto const* animation = mScene->mAnimations[mCurrentAnimation];
    auto const* pNodeAnimation = findNodeAnimation(animation, nodeName);

    if (pNodeAnimation)
    {
        glm::mat4 transMat = glm::mat4(1.f);
        glm::mat4 rotMat  = AnimationUtils::calcInterpolatedRotation(animationTime, pNodeAnimation);
        glm::mat4 scaleMat = glm::mat4(1.f);

        aiVector3D scaling = AnimationUtils::calcInterpolatedScaling(animationTime, pNodeAnimation);
        aiVector3D translation = AnimationUtils::calcInterpolatedPosition(animationTime, pNodeAnimation);

        scaleMat = glm::scale(scaleMat, glm::vec3(scaling.x, scaling.y, scaling.z));
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
    if (!mIsAnimated)
        return;
    float ticksPerSecond = mScene->mAnimations[mCurrentAnimation]->mTicksPerSecond;
    float timeInTicks = mAnimationTime * ticksPerSecond;
    float animTime = fmodf(timeInTicks, mScene->mAnimations[mCurrentAnimation]->mDuration - 1);
    readNodeHierarchy(animTime, mScene->mRootNode, glm::mat4(1.0f));
}

void	Mesh::setAnimation(Animation const& anim)
{
    mAnimationTime = anim.getTime();
    auto requiredAnimation = anim.getName();
    std::transform(requiredAnimation.begin(), requiredAnimation.end(), requiredAnimation.begin(), ::tolower);
    for (mCurrentAnimation = mScene->mNumAnimations - 1; mCurrentAnimation != 0; --mCurrentAnimation)
    {
        std::string animationName(mScene->mAnimations[mCurrentAnimation]->mName.C_Str());
        std::transform(animationName.begin(), animationName.end(), animationName.begin(), ::tolower);
        animationName = animationName.substr(animationName.find('|') + 1);
        if (animationName.find(requiredAnimation) != std::string::npos)
        {
            break;
        }
    }
}