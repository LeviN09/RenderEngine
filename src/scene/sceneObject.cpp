#include <glm/ext/matrix_transform.hpp>

#include "scene/sceneObject.hpp"

SceneObject::~SceneObject()
{
    m_renderer.RemoveObject(m_render_uid);
    m_physics_engine.RemoveObject(m_physics_uid);
}

void SceneObject::RemoveRenderObject()
{
    m_renderer.RemoveObject(m_render_uid);
}

void SceneObject::RemovePhysicsObject()
{
    m_physics_engine.RemoveObject(m_physics_uid);
}

const std::string& SceneObject::GetRenderUid() const
{
    return m_render_uid;
}

const std::string& SceneObject::GetPhysicsUid() const
{
    return m_physics_uid;
}

void SceneObject::SetPos(const glm::vec3& _pos)
{
    glm::vec3 pos = glm::vec3(m_model_mat[3]);

    m_model_mat = glm::translate(m_model_mat, _pos - pos);
}

const glm::vec3 SceneObject::GetPos()
{
    return glm::vec3(m_model_mat[3][0], m_model_mat[3][1], m_model_mat[3][2]);
}

void SceneObject::Translate(const glm::vec3& transform)
{
    m_model_mat = glm::translate(m_model_mat, transform);
}

void SceneObject::Rotate(float_t rad, const glm::vec3& rotate)
{
    m_model_mat = glm::rotate(m_model_mat, rad, rotate);
}

void SceneObject::Scale(const glm::vec3& scale)
{
    m_model_mat = glm::scale(m_model_mat, scale);
}

void SceneObject::Scale(float_t scale)
{
    Scale(glm::vec3(scale, scale, scale));
}