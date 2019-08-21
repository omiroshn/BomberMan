   
#include "ResourceManagement/Mesh.hpp"
#include "Utilities/AnimationUtils.h"
#include <imgui.h>

Mesh::Mesh(std::vector<Vertex>& vertices,
            std::vector<WeightData>& weights,
            std::vector<unsigned int>& indices,
            std::vector<std::shared_ptr<Texture>>& textures,
            std::map<std::string, unsigned int>& bones,
            std::vector<glm::mat4>& aOffsets,
            aiScene const* scene,
			float glossiness) :
    mVertices{std::move(vertices)}
    , mWeights{std::move(weights)}
    , mIndices{std::move(indices)}
    , mTextures{std::move(textures)}
    , mIsAnimated{scene->HasAnimations()}
    , mCurrentAnimation{0}
    , mGlossiness{glossiness}
    , mBones{std::move(bones)}
    , mOffsetMatrices{std::move(aOffsets)}
    , mScene{scene}
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
    glGenBuffers(2, &mVBO);

    glBindVertexArray(mVAO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(Vertex), &mVertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(unsigned int), &mIndices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);	
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    glEnableVertexAttribArray(1);	
    glVertexAttribPointer(1, 4, GL_INT_2_10_10_10_REV, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

    glEnableVertexAttribArray(2);	
    glVertexAttribPointer(2, 2, GL_SHORT, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

    glEnableVertexAttribArray(9);
    glVertexAttribPointer(9, 4, GL_INT_2_10_10_10_REV, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));

// weights is separate object

    if (mIsAnimated)
    {
        glGenBuffers(1, &mWBO);
        glBindBuffer(GL_ARRAY_BUFFER, mWBO);

        glBufferData(GL_ARRAY_BUFFER, mWeights.size() * sizeof(WeightData), &mWeights[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(7);
        glVertexAttribIPointer(7, 3, GL_UNSIGNED_BYTE, sizeof(WeightData), (void *)offsetof(WeightData, BonesID));

        glEnableVertexAttribArray(8);
        glVertexAttribPointer(8, 3, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(WeightData), (void *)offsetof(WeightData, Weighs));
    }

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

void Mesh::draw(std::shared_ptr<Shader> const &shader, std::vector<glm::mat4> const & transforms, glm::mat4 const & parentTransform)
{
	if (transforms.size() == 0)
		return;
    unsigned int diffuseNr  = 1;
    unsigned int normalNr   = 1;

	shader->use();
    for(unsigned int i = 0; i < mTextures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        std::string number;
        std::string name = mTextures[i]->getTextureType();
        if(name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if (name == "texture_normal")
            number = std::to_string(normalNr++);
        shader->setInt((name + number).c_str(), i);
        glBindTexture(GL_TEXTURE_2D, mTextures[i]->getTextureID());
    }
    glActiveTexture(GL_TEXTURE0);
    glBindBuffer(GL_ARRAY_BUFFER, mIBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * transforms.size(), &transforms[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    shader->setFloat("glossiness", mGlossiness);
    shader->setMat4("parentTransform", parentTransform);
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
        if (strcmp(nodeAnim->mNodeName.data, nodeName.data()) == 0)
            return nodeAnim;
    }
    return nullptr;
}


void	Mesh::readNodeHierarchy(float animationTime, aiNode const* node, const glm::mat4 parentTransform)
{

    std::string nodeName(node->mName.data);

    auto const* animation = mScene->mAnimations[mCurrentAnimation];
    auto const* pNodeAnimation = findNodeAnimation(animation, nodeName);

	glm::mat4 nodeTransform;
    if (pNodeAnimation)
    {

        aiVector3D scaling = AnimationUtils::calcInterpolatedScaling(animationTime, pNodeAnimation);
        aiVector3D translation = AnimationUtils::calcInterpolatedPosition(animationTime, pNodeAnimation);

        glm::mat4 transMat = glm::mat4(1.f);
        transMat = glm::translate(transMat, glm::vec3(translation.x, translation.y, translation.z));

        glm::mat4 transScaleMat = glm::scale(transMat, glm::vec3(scaling.x, scaling.y, scaling.z));
        glm::mat4 rotMat  = AnimationUtils::calcInterpolatedRotation(animationTime, pNodeAnimation);
		nodeTransform = transScaleMat * rotMat;
    }
	else
		nodeTransform = AnimationUtils::aiMatToGlmMat(node->mTransformation);

    glm::mat4 globalTransform = parentTransform * nodeTransform;
	auto It = mBones.find(nodeName);
    if (It != mBones.end())
    {
		unsigned int boneIndex = It->second;
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