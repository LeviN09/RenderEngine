#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "render/gpu_interface/EBO.hpp"
#include "render/gpu_interface/VAO.hpp"
#include "render/gpu_interface/VBO.hpp"
#include "render/gpu_interface/texture.hpp"
#include "render/renderObject.hpp"

void RenderObject::Render(const float& fov, const float& near, const float& far)
{
    m_shader->Activate();

    if (m_textures.size() > 0)
    {
        glUniform1i(glGetUniformLocation(m_shader->GetID(), "hasColorTexture"), m_has_color_texture ? 1 : 0);
        std::get<0>(m_textures[0])->Bind(GL_TEXTURE0);
    }

    m_vao->Bind();
    m_ebo->Bind();

    m_camera->Matrix(fov, near, far, *m_shader, "camMatrix");
    m_light->ExportLight(*m_shader);

    glUniform3fv(glGetUniformLocation(m_shader->GetID(), "viewPos"), 1, glm::value_ptr(m_camera->GetPosition()));
    glUniformMatrix4fv(glGetUniformLocation(m_shader->GetID(), "model"), 1, GL_FALSE, glm::value_ptr(m_model_mat));
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);

    if (m_textures.size() > 0)
    {
        std::get<0>(m_textures[0])->Unbind();
    }
}

void RenderObject::ConfigureVBO()
{
    GLfloat* tvbo = &m_vertices[0];

    m_vbo = std::make_unique<VBO>(tvbo, m_vertices.size() * sizeof(GLfloat));
}

void RenderObject::ConfigureEBO()
{
    GLuint* tebo = &m_indices[0];

    m_ebo = std::make_unique<EBO>(tebo, m_indices.size() * sizeof(GLuint));
}

void RenderObject::ConfigureVAO(const std::vector<VAOInfo>& attrs)
{
    m_vao = std::make_unique<VAO>();
    m_vao->Bind();
    for (auto& attr : attrs)
    {
        m_vao->LinkAttrib(*m_vbo, attr.layout, attr.numComponents, attr.type, attr.stride, (void*)attr.offset);
    }
}

void RenderObject::Configure()
{
    ConfigureVBO();
    ConfigureVAO(VAOInfo::GetVAOInfo(m_has_color, m_has_normals, m_has_color_texture));
    ConfigureEBO();
}

void RenderObject::SetCamera(const std::shared_ptr<Camera>& camera)
{
    m_camera = camera;
}

void RenderObject::SetLight(const std::shared_ptr<Light>& light)
{
    m_light = light;
}

void RenderObject::AddShader()
{
    m_shader = std::make_unique<Shader>("../shaders/default.vert", "../shaders/default.frag");
}

void RenderObject::AddTexture(std::unique_ptr<Texture> texture, const std::string& texUni)
{
    m_vao->Bind();
    m_has_color_texture = true;
    m_textures.push_back(std::make_tuple(std::move(texture), texUni));
    texture->TexUnit(*m_shader, texUni.c_str(), 0);
}

void RenderObject::PushToVerts(const std::initializer_list<float_t>& coords)
{
    for (const auto& coord : coords)
    {
        m_vertices.push_back(coord);
    }
}

void RenderObject::PushToInds(const std::initializer_list<int64_t>& coords)
{
    for (const auto& coord : coords)
    {
        m_indices.push_back(coord);
    }
}