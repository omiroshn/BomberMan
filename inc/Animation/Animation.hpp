//
// Created by Vadim on 2019-04-05.
//

#ifndef BOMBERMAN_ANIMATION_HPP
#define BOMBERMAN_ANIMATION_HPP

#include <vector>
#include "KeyFrame.hpp"

class Animation
{
public:
    float                   mLength;
    std::vector<KeyFrame>   mKeyFrames;

    Animation(float lengthInSeconds, std::vector<KeyFrame> const& frames);
    float getLength() const;
    std::vector<KeyFrame> getKeyFrames() const;
};

#endif //BOMBERMAN_ANIMATION_HPP
