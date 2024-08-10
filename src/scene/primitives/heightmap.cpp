#include "scene/primitives/heightmap.hpp"
#include "physics/physicsEngine.hpp"
#include "render/renderer.hpp"
#include "scene/primitives/object_builders/heightmapParts.hpp"

void HeightmapObject::AddRenderObject(const ShaderType& type)
{
    std::unique_ptr<HeightmapRender> temp = std::make_unique<HeightmapRender>(m_render_uid, m_model_mat, m_scale, m_resolution, m_displacement, m_sampler, true);
    m_renderer.AddObject(std::move(temp), type);
}

void HeightmapObject::AddRenderObject()
{
    std::unique_ptr<HeightmapRender> temp = std::make_unique<HeightmapRender>(m_render_uid, m_model_mat, m_scale, m_resolution, m_displacement, m_sampler, true);
    m_renderer.AddObject(std::move(temp));
}

void HeightmapObject::AddPhysicsObject()
{
    std::unique_ptr<HeightmapBody> temp = std::make_unique<HeightmapBody>(m_physics_uid, m_model_mat, m_scale, 1.0f, m_sampler);
    m_physics_engine.AddObject(std::move(temp));
}