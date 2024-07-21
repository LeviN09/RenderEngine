#include "scene/primitives/plane.hpp"
#include "scene/primitives/object_builders/planeParts.hpp"

void PlaneObject::AddRenderObject(const std::string& vert_shader, const std::string& frag_shader)
{
    std::unique_ptr<PlaneRender> temp = std::make_unique<PlaneRender>(m_render_uid, m_model_mat);
    m_renderer.AddObject(std::move(temp), vert_shader, frag_shader);
}

void PlaneObject::AddRenderObject()
{
    std::unique_ptr<PlaneRender> temp = std::make_unique<PlaneRender>(m_render_uid, m_model_mat);
    m_renderer.AddObject(std::move(temp));
}

void PlaneObject::AddPhysicsObject()
{
    //std::unique_ptr<PlaneBody> temp = std::make_unique<PlaneBody>(m_uid, );
    //m_physics_engine.AddObject(std::move(temp));
}