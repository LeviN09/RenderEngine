#ifndef SPHERE_CLASS_HPP
#define SPHERE_CLASS_HPP

#include <glm/ext/matrix_float4x4.hpp>

#include "physics/physicsEngine.hpp"
#include "render/renderer.hpp"
#include "scene/sceneObject.hpp"

class SphereObject : public SceneObject
{
    public:
        SphereObject(Renderer& renderer, PhysicsEngine& physics_engine, const std::string& uid, const glm::vec3& pos, float_t radius):
            SceneObject(renderer, physics_engine, uid, pos), m_radius{ radius }
        {}
        ~SphereObject() {}

        void AddRenderObject() override;
        void AddPhysicsObject() override;

    private:
        float_t m_mass{ 1.0f };
        float_t m_radius{ 1.0f };
};

#endif