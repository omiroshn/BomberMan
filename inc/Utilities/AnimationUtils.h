//
// Created by Vadym KOZLOV on 2019-06-28.
//

#ifndef BOMBERMAN_ANIMATIONUTILS_H
#define BOMBERMAN_ANIMATIONUTILS_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <glm/glm.hpp>

namespace AnimationUtils
{
    glm::mat4			aiMatToGlmMat(aiMatrix4x4 ai);
    glm::mat4           quatToMat(const aiQuaternion &quat);
    aiVector3D          calcInterpolatedPosition(float animationTime, const aiNodeAnim *node);
    aiVector3D          calcInterpolatedScaling(double animationTime, const aiNodeAnim *node);
    glm::mat4           calcInterpolatedRotation(double animationTime, const aiNodeAnim *node);
};


#endif //BOMBERMAN_ANIMATIONUTILS_H
