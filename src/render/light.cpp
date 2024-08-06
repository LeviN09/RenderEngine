#include <glm/ext/quaternion_transform.hpp>
#include <glm/geometric.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "render/light.hpp"
#include "render/gpu_interface/shaderClass.hpp"

#include <GL/gl.h>
#include <iostream>

void Light::Translate(const glm::vec3& translate)
{
    m_position += translate;
}

void Light::SetPosition(const glm::vec3& position)
{
    m_position = position;
}

void DirectionalLight::RenderShadowMap()
{
    m_shadow_shader->Activate();

    glUniformMatrix4fv(glGetUniformLocation(m_shadow_shader->GetID(), "lightProjection"), 1, GL_FALSE, glm::value_ptr(m_light_projection));

    glViewport(0, 0, m_shadowmap_width, m_shadowmap_height);
    glBindFramebuffer(GL_FRAMEBUFFER, m_shadowmap_fbo);
    glClear(GL_DEPTH_BUFFER_BIT);
}

const GLuint& DirectionalLight::GetShadowMap()
{
    return m_shadowmap;
}

Shader& DirectionalLight::GetShadowShader()
{
    return *m_shadow_shader.get();
}

void DirectionalLight::ExportLight(Shader& shader)
{
    UpdateLightProjection();
    glUniform3fv(glGetUniformLocation(shader.GetID(), "dirLightDir"), 1, glm::value_ptr(m_direction));
    glUniform3fv(glGetUniformLocation(shader.GetID(), "dirLightColor"), 1, glm::value_ptr(m_color));
    glUniform1f(glGetUniformLocation(shader.GetID(), "dirLightIntensity"), m_intensity);
    glUniformMatrix4fv(glGetUniformLocation(shader.GetID(), "lightProjection"), 1, GL_FALSE, glm::value_ptr(m_light_projection));
}

void DirectionalLight::SetDirection(const glm::vec3& direction)
{
    m_direction = glm::normalize(direction);
}

void DirectionalLight::Rotate(float_t rad, const glm::vec3& rotate)
{
    SetDirection(glm::rotate(glm::mat4(1.0f), rad, rotate) * glm::vec4(m_direction, 1.0f));
}

const glm::vec3& DirectionalLight::GetDirection() const
{
    return m_direction;
}

void DirectionalLight::InitShadowMap()
{
    glGenFramebuffers(1, &m_shadowmap_fbo);

    glGenTextures(1, &m_shadowmap);
    glBindTexture(GL_TEXTURE_2D, m_shadowmap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_shadowmap_width, m_shadowmap_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float_t clamp_color[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, clamp_color);

    glBindFramebuffer(GL_FRAMEBUFFER, m_shadowmap_fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_shadowmap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    m_orthogonal_projection = glm::ortho(-m_projection_size, m_projection_size, -m_projection_size, m_projection_size, 0.1f, m_projection_size * 2.0f);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
	    std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    }
}

void DirectionalLight::UpdateLightProjection()
{
    m_light_view = glm::lookAt(20.0f * m_direction + m_position, m_position, glm::vec3(0.0f, 1.0f, 0.0f));
    m_light_projection = m_orthogonal_projection * m_light_view;
}

void PointLight::ExportLight(Shader& shader)
{
    glUniform3fv(glGetUniformLocation(shader.GetID(), "lightPos"), 1, glm::value_ptr(m_position));
    glUniform3fv(glGetUniformLocation(shader.GetID(), "lightColor"), 1, glm::value_ptr(m_color));
    glUniform1f(glGetUniformLocation(shader.GetID(), "lightIntensity"), m_intensity);
}