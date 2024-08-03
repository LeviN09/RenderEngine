#include <glm/geometric.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "render/light.hpp"
#include "render/gpu_interface/shaderClass.hpp"

void Light::Translate(const glm::vec3& translate)
{
    m_position += translate;
}

void DirectionalLight::ExportLight(const Shader& shader)
{
    glUniform3fv(glGetUniformLocation(shader.GetID(), "dirLightColor"), 1, glm::value_ptr(m_color));
    glUniform3fv(glGetUniformLocation(shader.GetID(), "dirLightIntensity"), 1, glm::value_ptr(m_intensity));
    glUniform3fv(glGetUniformLocation(shader.GetID(), "dirLightDir"), 1, glm::value_ptr(m_direction));
}

void DirectionalLight::SetDirection(const glm::vec3& direction)
{
    m_direction = glm::normalize(direction);
}

const glm::vec3& DirectionalLight::GetDirection() const
{
    return m_direction;
}

void PointLight::ExportLight(const Shader& shader)
{
    glUniform3fv(glGetUniformLocation(shader.GetID(), "lightPos"), 1, glm::value_ptr(m_position));
    glUniform3fv(glGetUniformLocation(shader.GetID(), "lightColor"), 1, glm::value_ptr(m_color));
}