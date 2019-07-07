//
// Created by Vadim on 2019-02-24.
//

#include "Utilities/AABB.hpp"

AABB::AABB()
{
    setNull();
}

AABB::AABB(const AABB& aabb)
{
    setNull();
    extend(aabb);
}

AABB::~AABB()
{
}

void AABB::setNull()
{
    mMin = glm::vec3(1.0); mMax = glm::vec3(-1.0f);
}

bool AABB::isNull() const
{
    return mMin.x > mMax.x || mMin.y > mMax.y || mMin.z > mMax.z;
}

void AABB::extend(float val)
{
    if (!isNull())
    {
        mMin -= glm::vec3(val);
        mMax += glm::vec3(val);
    }
}

void AABB::extend(glm::vec3 const& p)
{
    if (!isNull())
    {
        mMin = glm::min(p, mMin);
        mMax = glm::max(p, mMax);
    }
    else
    {
        mMin = p;
        mMax = p;
    }
}

void AABB::extend(AABB const& aabb)
{
    if (!aabb.isNull())
    {
        extend(aabb.mMin);
        extend(aabb.mMax);
    }
}

void AABB::operator+=(float val)
{
    extend(val);
};

void AABB::operator+=(glm::vec3 const& p)
{
    extend(p);
};

void AABB::operator+=(AABB const &aabb)
{
    extend(aabb);
};


glm::vec3 AABB::getMin() const
{
    return mMin;
}

glm::vec3 AABB::getMax() const
{
    return mMax;
}

glm::vec3 AABB::getDiagonal() const
{
    if (!isNull())
        return mMax - mMin;
    else
        return glm::vec3(0);
}

glm::vec3 AABB::getCenter() const
{
    if (!isNull())
    {
        glm::vec3 d = getDiagonal();
        return mMin + (d * 0.5f);
    }
    else
    {
        return glm::vec3(0.0);
    }
}

bool AABB::intersects(AABB const& b) const
{
    if (isNull() || b.isNull())
        return false;

    if ((mMax.x < b.mMin.x) || (mMin.x > b.mMax.x) ||
        (mMax.y < b.mMin.y) || (mMin.y > b.mMax.y) ||
        (mMax.z < b.mMin.z) || (mMin.z > b.mMax.z))
    {
        return false;
    }
    return true;
}

bool AABB::isSimilarTo(AABB const& b, float diff) const
{
    if (isNull() || b.isNull()) return false;

    glm::vec3 acceptable_diff=( (getDiagonal()+b.getDiagonal()) / 2.0f)*diff;
    glm::vec3 min_diff(mMin-b.mMin);
    min_diff = glm::vec3(fabs(min_diff.x),fabs(min_diff.y),fabs(min_diff.z));
    if (min_diff.x > acceptable_diff.x) return false;
    if (min_diff.y > acceptable_diff.y) return false;
    if (min_diff.z > acceptable_diff.z) return false;
    glm::vec3 max_diff(mMax-b.mMax);
    max_diff = glm::vec3(fabs(max_diff.x),fabs(max_diff.y),fabs(max_diff.z));
    if (max_diff.x > acceptable_diff.x) return false;
    if (max_diff.y > acceptable_diff.y) return false;
    if (max_diff.z > acceptable_diff.z) return false;
    return true;
}

bool AABB::operator==(AABB const& aabb)
{
    return isSimilarTo(aabb);
};

AABB AABB::transform(glm::mat4 const & aTransform)
{
    AABB retVal;
    glm::vec4 min4(mMin, 0);
    glm::vec4 max4(mMax, 0);

    retVal += min4 * aTransform;
    retVal += max4 * aTransform;

    return retVal;
}

glm::vec3 AABB::size() const
{
    glm::vec3 s;

    s.x = mMax.x - mMin.x;
    s.y = mMax.y - mMin.y;
    s.z = mMax.z - mMin.z;
    return s;
}
