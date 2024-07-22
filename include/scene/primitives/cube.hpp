#ifndef CUBE_CLASS_HPP
#define CUBE_CLASS_HPP

#include <cmath>

#include "physics/physicsEngine.hpp"
#include "render/gpu_interface/shaderClass.hpp"
#include "render/renderer.hpp"
#include "scene/sceneObject.hpp"

class CubeObject : public SceneObject
{
    public:
        CubeObject(Renderer& renderer, PhysicsEngine& physics_engine, const std::string& uid, const glm::vec3& pos, const glm::vec3& scale):
            SceneObject(renderer, physics_engine, uid, pos), m_scale{ scale }
        {}
        CubeObject(Renderer& renderer, PhysicsEngine& physics_engine, const std::string& uid, const glm::vec3& pos, float_t scale_x, float_t scale_y, float_t scale_z):
            SceneObject(renderer, physics_engine, uid, pos), m_scale{ scale_x, scale_y, scale_z }
        {}
        CubeObject(Renderer& renderer, PhysicsEngine& physics_engine, const std::string& uid, const glm::vec3& pos, float_t scale):
            SceneObject(renderer, physics_engine, uid, pos), m_scale{ scale }
        {}
        CubeObject(Renderer& renderer, PhysicsEngine& physics_engine, const std::string& uid, glm::mat4& model_mat, float_t scale):
            SceneObject(renderer, physics_engine, uid, model_mat), m_scale{ scale }
        {}
        ~CubeObject() {}

        void AddRenderObject(const ShaderType& type) override;
        void AddRenderObject() override;
        void AddPhysicsObject() override;

    private:
        glm::vec3 m_scale;
};

#endif