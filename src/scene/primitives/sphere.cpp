#include "scene/primitives/sphere.hpp"
#include "scene/primitives/object_builders/sphereParts.hpp"
#include <cstdint>
#include <memory>

void SphereObject::AddRenderObject()
{
    std::unique_ptr<SphereRender> temp = std::make_unique<SphereRender>(m_render_uid, m_model_mat, m_radius, (int64_t)m_radius * 5, (int64_t)m_radius * 5);
    m_renderer.AddObject(std::move(temp));
}

void SphereObject::AddPhysicsObject()
{
    std::unique_ptr<SphereBody> temp = std::make_unique<SphereBody>(m_physics_uid, m_model_mat, m_radius, m_mass);
    m_physics_engine.AddObject(std::move(temp));
}