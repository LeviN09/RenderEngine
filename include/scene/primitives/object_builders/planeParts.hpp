#pragma once

#include <cmath>

#include "physics/physicsObject.hpp"
#include "render/renderObject.hpp"

class PlaneRender : public RenderObject
{
    public:
        PlaneRender(const std::string& uid, const glm::mat4& model_mat, const float_t& scale):
            RenderObject(uid, model_mat), m_scale{ scale }
        {
            Init();
        }

        void Init() override;
        void Update(const double_t& delta_time) override {}

    private:
        float_t m_scale;
};

class PlaneBody : public PhysicsObject
{
    public:
        PlaneBody(const std::string& uid, glm::mat4& model_mat, const float_t& scale, const float_t& mass):
            PhysicsObject(uid, model_mat, mass), m_scale{ scale }
        {}

        const bool IsCollidingWith(const PhysicsObject& other) const override;
        const bool IsCollidingWith(const SphereBody& other) const override;
        const bool IsCollidingWith(const PlaneBody& other) const override;
        const bool IsCollidingWith(const CubeBody& other) const override;

        void CalcCollision(PhysicsObject& other) override;
        void CalcCollision(SphereBody& other) override;
        void CalcCollision(CubeBody& other) override;

    private:
        float_t m_scale;
};