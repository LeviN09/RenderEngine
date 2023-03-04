#ifndef PHYSICS_ENGINE_HPP
#define PHYSICS_ENGINE_HPP

#include "physicsObject.hpp"
#include "renderer.hpp"
#include "sphere.hpp"
#include <glm/ext/vector_float3.hpp>
#include <vector>
class PhysicsEngine {
    private:
        Renderer* renderer;
        std::vector<PhysicsObject*> objects;

        void CollisionUpdate();
        //void CalcCollision(PhysicsObject* a, PhysicsObject* b) {}
        void CalcCollision(SphereBody* a, SphereBody* b);
        void GravityUpdate();

    public:
        PhysicsEngine(Renderer* _renderer): renderer(_renderer) {}

        void AddObject(PhysicsObject* object);
        void Update();
};

#endif