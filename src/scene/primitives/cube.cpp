#include "scene/primitives/cube.hpp"
#include "scene/primitives/object_builders/cubeParts.hpp"

void CubeObject::AddRenderObject()
{
    std::unique_ptr<CubeRender> temp = std::make_unique<CubeRender>(m_render_uid, m_model_mat, m_scale);
    m_renderer.AddObject(std::move(temp));
}

void CubeObject::AddPhysicsObject()
{
    //std::unique_ptr<CubeBody> temp = std::make_unique<CubeBody>(m_uid, );
    //m_physics_engine.AddObject(std::move(temp));
}