#include <cstdint>
#include <memory>

#include "scene/primitives/object_builders/sphereParts.hpp"
#include "scene/primitives/sphere.hpp"

void SphereObject::AddRenderObject(const ShaderType& type)
{
    uint64_t detail_level = s_min_detail;
    uint64_t calculated_detail = m_radius * s_scale_ratio;
    if (calculated_detail > s_min_detail)
    {
        detail_level = calculated_detail;
    }
    if (calculated_detail > s_max_detail)
    {
        detail_level = s_max_detail;
    }

    std::unique_ptr<SphereRender> temp = std::make_unique<SphereRender>(m_render_uid, m_model_mat, m_radius, detail_level, detail_level);
    m_renderer.AddObject(std::move(temp), type);
}

void SphereObject::AddRenderObject()
{
    uint64_t detail_level = s_min_detail;
    uint64_t calculated_detail = m_radius * s_scale_ratio;
    if (calculated_detail > s_min_detail)
    {
        detail_level = calculated_detail;
    }
    if (calculated_detail > s_max_detail)
    {
        detail_level = s_max_detail;
    }

    std::unique_ptr<SphereRender> temp = std::make_unique<SphereRender>(m_render_uid, m_model_mat, m_radius, detail_level, detail_level);
    m_renderer.AddObject(std::move(temp));
}

void SphereObject::AddPhysicsObject()
{
    std::unique_ptr<SphereBody> temp = std::make_unique<SphereBody>(m_physics_uid, m_model_mat, m_radius, m_mass);
    m_physics_engine.AddObject(std::move(temp));
}