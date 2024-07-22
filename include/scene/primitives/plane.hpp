#ifndef PLANE_CLASS_HPP
#define PLANE_CLASS_HPP

#include "render/gpu_interface/shaderClass.hpp"
#include "render/renderer.hpp"
#include "scene/sceneObject.hpp"

class PlaneObject : public SceneObject
{
    public:
        PlaneObject(Renderer& renderer, PhysicsEngine& physics_engine, const std::string& uid, const glm::vec3& pos, float_t scale):
            SceneObject(renderer, physics_engine, uid, pos), m_scale{ scale }
        {}
        PlaneObject(Renderer& renderer, PhysicsEngine& physics_engine, const std::string& uid, glm::mat4& model_mat, float_t scale):
            SceneObject(renderer, physics_engine, uid, model_mat), m_scale{ scale }
        {}
        ~PlaneObject() {}

        void AddRenderObject(const ShaderType& type) override;
        void AddRenderObject() override;
        void AddPhysicsObject() override;

    private:
        float_t m_scale{ 1.0f };
};

#endif