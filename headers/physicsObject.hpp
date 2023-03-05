#ifndef PHYSICS_OBJECT_HPP
#define PHYSICS_OBJECT_HPP

#include "renderObject.hpp"
class PhysicsObject {
    protected:
        RenderObject* renderObj;
        bool hasCollision{true}, hasGravity{false};

    public:
        glm::vec3 position;
        glm::vec3 constVelocity{0.0f};
        glm::vec3 velocity{0.0f};
        glm::vec3 gravityAcc{0.0f};
        glm::vec3 normalAcc{0.0f};
        glm::vec3 acceleration{0.0f};
        float mass;
        bool isColliding;

        PhysicsObject(glm::vec3 _position, float _mass):
        position(_position), mass(_mass) {}

        RenderObject* GetRenderObject();
        glm::vec3 SumAcceleration();
        void Push(glm::vec3 _push);
        void SetVelocity(glm::vec3 _vel);
        void AddVelocity(glm::vec3 _vel);
        void SetCollision(bool _hasCollision);
        bool GetCollision();
        void SetGravity(bool _hasGravity);
        bool GetGravity();
};

#endif