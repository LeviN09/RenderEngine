#include "scene/primitives/cube.hpp"
#include "scene/primitives/object_builders/cubeParts.hpp"

void CubeObject::AddRenderObject(const ShaderType& type)
{
    std::unique_ptr<CubeRender> temp = std::make_unique<CubeRender>(m_render_uid, m_model_mat, m_scale, false);
    m_renderer.AddObject(std::move(temp), type);
}

void CubeObject::AddRenderObject()
{
    std::unique_ptr<CubeRender> temp = std::make_unique<CubeRender>(m_render_uid, m_model_mat, m_scale, false);
    m_renderer.AddObject(std::move(temp));
}

void CubeObject::AddPhysicsObject()
{
    std::unique_ptr<CubeBody> temp = std::make_unique<CubeBody>(m_physics_uid, m_model_mat, m_scale, m_mass);
    m_physics_engine.AddObject(std::move(temp));
}