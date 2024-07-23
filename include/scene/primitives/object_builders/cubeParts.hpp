#ifndef CUBEPARTS_CLASS_HPP
#define CUBEPARTS_CLASS_HPP

#include <cmath>

#include "physics/physicsObject.hpp"
#include "render/renderObject.hpp"

class CubeRender : public RenderObject
{

    public:
        CubeRender(const std::string& uid, const glm::mat4& model_mat, const glm::vec3& scale):
            RenderObject(uid, model_mat), m_scale{ scale }
        {
            Init();
            Configure();
        }

    private:
        void Init() override;
        void Update(const double_t& delta_time) override;

        glm::vec3 m_scale;
};

class CubeBody : public PhysicsObject
{
    public:
        CubeBody(const std::string& uid, glm::mat4& model_mat, float_t mass):
            PhysicsObject(uid, model_mat, mass)
        {}

        void CalcCollision(PhysicsObject& other) override;
        void CalcCollision(SphereBody& other) override;
};

#endif