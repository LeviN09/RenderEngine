#pragma once

#include <cmath>
#include <cstdint>
#include <vector>

#include "physics/physicsObject.hpp"
#include "render/renderObject.hpp"

class CubeRender : public RenderObject
{

    public:
        CubeRender(const std::string& uid, const glm::mat4& model_mat, const glm::vec3& scale, bool is_shared):
            RenderObject(uid, model_mat), m_scale{ scale }, m_is_shared{ is_shared }
        {
            Init();
        }

    private:
        void Init() override;
        void InitSixSided();
        void InitPlane(const uint64_t plane_num);
        void InitShared();
        void Update(const double_t& delta_time) override;

        bool m_is_shared{ false };
        glm::vec3 m_scale;
        std::vector<glm::vec3> m_points;
        std::vector<glm::uvec4> m_inds;
        std::vector<glm::vec3> m_norms;
};

class CubeBody : public PhysicsObject
{
    public:
        CubeBody(const std::string& uid, glm::mat4& model_mat, const glm::vec3& scale, float_t mass):
            PhysicsObject(uid, model_mat, mass), m_scale{ scale }
        {}

        const glm::vec3& GetHalfExtent() const;

        const bool IsCollidingWith(const PhysicsObject& other) const override;
        const bool IsCollidingWith(const SphereBody& other) const override;
        const bool IsCollidingWith(const PlaneBody& other) const override;
        const bool IsCollidingWith(const CubeBody& other) const override;

        void CalcCollision(PhysicsObject& other) override;
        void CalcCollision(SphereBody& other) override;
        void CalcCollision(CubeBody& other) override;

    private:
        glm::vec3 m_scale;
        glm::vec3 m_half_extent{ m_scale / 2.0f };
};