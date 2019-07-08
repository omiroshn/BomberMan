//
// Created by Vadim on 2019-02-24.
//

#ifndef BOMBERMAN_AABB_HPP
#define BOMBERMAN_AABB_HPP

#include <glm/glm.hpp>

class AABB
{
    public:
        AABB();
        AABB(AABB const& aabb);
        ~AABB();

        void setNull();
        bool isNull() const;

        void extend(float val);
        void extend(glm::vec3 const& p);
        void extend(AABB const& aabb);

        void operator+=(AABB const & aabb);
        void operator+=(float val);
        void operator+=(glm::vec3 const& p);
        glm::vec3 size() const;
        /// Retrieves the center of the AABB.
        glm::vec3 getCenter() const;
        glm::vec3 getDiagonal() const;
        glm::vec3 getMin() const;
        glm::vec3 getMax() const;

        bool intersects(AABB const& aabb) const;

        bool isSimilarTo(AABB const& aabb, float diff = 0.5f) const;
        bool operator==(AABB const& aabb);

        AABB transform(glm::mat4 const & aTransform);

    private:
        glm::vec3 mMin;
        glm::vec3 mMax;

};

#endif //BOMBERMAN_AABB_HPP
