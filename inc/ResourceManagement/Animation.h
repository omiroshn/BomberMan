//
// Created by Vadym KOZLOV on 2019-06-28.
//

#ifndef BOMBERMAN_ANIMATION_H
#define BOMBERMAN_ANIMATION_H
#include <string>

enum class AnimationType
{
    Idle,
    Running,
    Dying
};

class Animation
{
public:
    explicit Animation(AnimationType = AnimationType::Idle);
    Animation(double time, AnimationType = AnimationType::Idle);
    ~Animation() = default;

    double          getTime() const;
    void            setTime(double time);
    AnimationType   getType() const;
    void            setType(AnimationType mType);
    std::string     getName() const;
    void            tick(float DeltaTime);
private:
    double mTime;
    AnimationType mType;
};


#endif //BOMBERMAN_ANIMATION_H
