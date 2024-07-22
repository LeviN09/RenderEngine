#ifndef SPHERE_CLASS_HPP
#define SPHERE_CLASS_HPP

#include <glm/ext/matrix_float4x4.hpp>

#include "physics/physicsEngine.hpp"
#include "render/gpu_interface/shaderClass.hpp"
#include "render/renderer.hpp"
#include "scene/sceneObject.hpp"

class SphereObject : public SceneObject
{
    public:
        SphereObject(Renderer& renderer, PhysicsEngine& physics_engine, const std::string& uid, const glm::vec3& pos):
            SceneObject(renderer, physics_engine, uid, pos)
        {}

        SphereObject(Renderer& renderer, PhysicsEngine& physics_engine, const std::string& uid, const glm::vec3& pos, float_t radius):
            SceneObject(renderer, physics_engine, uid, pos), m_radius{ radius }
        {}

        SphereObject(Renderer& renderer, PhysicsEngine& physics_engine, const std::string& uid, const glm::vec3& pos, float_t radius, float_t mass):
            SceneObject(renderer, physics_engine, uid, pos), m_radius{ radius }, m_mass{ mass }
        {}
        ~SphereObject() {}

        void AddRenderObject(const ShaderType& type) override;
        void AddRenderObject() override;
        void AddPhysicsObject() override;

    private:
        float_t m_mass{ 100.0f };
        float_t m_radius{ 1.0f };

        static const uint64_t s_scale_ratio{ 10 }, s_min_detail{ 8 };
};

#endif