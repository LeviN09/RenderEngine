#ifndef SCENEOBJECT_CLASS_HPP
#define SCENEOBJECT_CLASS_HPP

#include <cmath>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float3.hpp>

#include "render/gpu_interface/shaderClass.hpp"
#include "scene/idTag.hpp"

class PhysicsEngine;
class Renderer;

class SceneObject : public IdTag
{
    public:
        ~SceneObject() = default;

        virtual void AddRenderObject(const ShaderType& type) = 0;
        virtual void AddRenderObject() = 0;
        virtual void AddPhysicsObject() = 0;

        const std::string& GetRenderUid() const;
        const std::string& GetPhysicsUid() const;

        void SetPos(const glm::vec3& pos);
        void Translate(const glm::vec3& transform);
        void Rotate(float_t rad, const glm::vec3& rotate);
        void Scale(float_t scale);
        void Scale(const glm::vec3& scale);
    
    protected:
        SceneObject(Renderer& renderer, PhysicsEngine& physics_engine, const std::string& uid):
            IdTag(uid), m_renderer{ renderer }, m_physics_engine{ physics_engine }, m_model_mat(1.0f), m_render_uid{ "r_" + uid }, m_physics_uid{ "p_" + uid }
        {}
        SceneObject(Renderer& renderer, PhysicsEngine& physics_engine, const std::string& uid, const glm::vec3& pos):
            IdTag(uid), m_renderer{ renderer }, m_physics_engine{ physics_engine }, m_model_mat(1.0f), m_render_uid{ "r_" + uid }, m_physics_uid{ "p_" + uid }
        {
            Translate(pos);
        }
        SceneObject(Renderer& renderer, PhysicsEngine& physics_engine, const std::string& uid, const glm::mat4& model_mat):
            IdTag(uid), m_renderer{ renderer }, m_physics_engine{ physics_engine }, m_model_mat(model_mat), m_render_uid{ "r_" + uid }, m_physics_uid{ "p_" + uid }
        {}

        std::string m_render_uid;
        std::string m_physics_uid;

        glm::mat4 m_model_mat;

        Renderer& m_renderer;
        PhysicsEngine& m_physics_engine;
};

#endif