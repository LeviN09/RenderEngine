#include "scene/primitives/sphere.hpp"
#include "scene/primitives/object_builders/sphereParts.hpp"
#include <cstdint>
#include <memory>

void SphereObject::AddRenderObject(const ShaderType& type)
{
    uint64_t detail_level = s_min_detail;
    if (m_radius * s_scale_ratio > s_min_detail)
    {
        detail_level = m_radius * s_scale_ratio;
    }

    std::unique_ptr<SphereRender> temp = std::make_unique<SphereRender>(m_render_uid, m_model_mat, m_radius, detail_level, detail_level);
    m_renderer.AddObject(std::move(temp), type);
}

void SphereObject::AddRenderObject()
{
    uint64_t detail_level = s_min_detail;
    if (m_radius * s_scale_ratio > s_min_detail)
    {
        detail_level = m_radius * s_scale_ratio;
    }

    std::unique_ptr<SphereRender> temp = std::make_unique<SphereRender>(m_render_uid, m_model_mat, m_radius, detail_level, detail_level);
    m_renderer.AddObject(std::move(temp));
}

void SphereObject::AddPhysicsObject()
{
    std::unique_ptr<SphereBody> temp = std::make_unique<SphereBody>(m_physics_uid, m_model_mat, m_radius, m_mass);
    m_physics_engine.AddObject(std::move(temp));
}