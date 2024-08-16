#pragma once

#include <cmath>
#include <cstdint>
#include <functional>

#include "physics/physicsObject.hpp"
#include "render/renderObject.hpp"

class HeightmapRender : public RenderObject
{
    public:
        HeightmapRender(const std::string& uid, const glm::mat4& model_mat, const float_t& scale, const uint64_t& resolution, const glm::vec2& displacement, const std::function<float_t(float_t, float_t)>& sampler, bool is_shared):
            RenderObject(uid, model_mat), m_scale{ scale }, m_resolution{ resolution }, m_displacement{ displacement }, m_sampler{ sampler }, m_is_shared{ is_shared }
        {
            Init();
        }
        HeightmapRender(const std::string& uid, const glm::mat4& model_mat, const float_t& scale, const uint64_t& resolution, const std::function<float_t(float_t, float_t)>& sampler, bool is_shared):
            RenderObject(uid, model_mat), m_scale{ scale }, m_resolution{ resolution }, m_sampler{ sampler }, m_is_shared{ is_shared }
        {
            Init();
        }

        void Init() override;
        void InitSeparate();
        void InitShared();
        void Update(const double_t& delta_time) override {}

        const uint64_t& GetResolution();
        void ChangeResolution(const uint64_t& resolution);

    private:
        glm::vec3 GetNormal(float_t x, float_t y);

        float_t m_scale;
        uint64_t m_resolution;
        glm::vec2 m_displacement{ 0.0f };
        std::function<float_t(float_t, float_t)> m_sampler;

        bool m_is_shared{ true };
        const glm::vec3 m_up{ 0.0f, 1.0f, 0.0f};
};

class HeightmapBody : public PhysicsObject
{
    public:
        HeightmapBody(const std::string& uid, glm::mat4& model_mat, const float_t& scale, const float_t& mass, const std::function<float_t(uint64_t, uint64_t)>& sampler):
            PhysicsObject(uid, model_mat, mass), m_scale{ scale }, m_sampler{ sampler }
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
        std::function<float_t(uint64_t, uint64_t)> m_sampler;
};