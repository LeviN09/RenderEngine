#ifndef CUBE_CLASS_HPP
#define CUBE_CLASS_HPP

#include "physics/physicsEngine.hpp"
#include "render/renderer.hpp"
#include "scene/sceneObject.hpp"

class CubeObject : public SceneObject
{
    public:
        CubeObject(Renderer& renderer, PhysicsEngine& physics_engine, const std::string& uid, const glm::vec3& pos, float_t scale):
            SceneObject(renderer, physics_engine, uid, pos), m_scale{ scale }
        {}
        CubeObject(Renderer& renderer, PhysicsEngine& physics_engine, const std::string& uid, glm::mat4& model_mat, float_t scale):
            SceneObject(renderer, physics_engine, uid, model_mat), m_scale{ scale }
        {}
        ~CubeObject() {}

        void AddRenderObject() override;
        void AddPhysicsObject() override;

    private:
        float_t m_scale;
};

#endif