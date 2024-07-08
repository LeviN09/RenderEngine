#ifndef PHYSICS_ENGINE_HPP
#define PHYSICS_ENGINE_HPP

#include <functional>
#include <glm/ext/vector_float3.hpp>
#include <memory>
#include <vector>

#include "physics/physicsObject.hpp"
#include "scene/iTaggedObjectManager.hpp"
#include "scene/primitives/object_builders/sphereParts.hpp"

class PhysicsEngine : public ITaggedObjectManager
{
    public:
        PhysicsEngine() = default;
        ~PhysicsEngine() = default;

        std::optional<std::reference_wrapper<IdTag>> GetObject(const std::string& uid) const override;

        void AddObject(std::unique_ptr<PhysicsObject> object);
        void Update();

    private:
        void CollisionUpdate();
        void CalcCollision(SphereBody& a, const SphereBody& b);
        void GravityUpdate();

        std::vector<std::unique_ptr<PhysicsObject>> m_objects;
};

#endif