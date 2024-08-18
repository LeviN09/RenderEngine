#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>

#include "render/gpu_interface/EBO.hpp"
#include "render/gpu_interface/VAO.hpp"
#include "render/gpu_interface/VBO.hpp"
#include "render/gpu_interface/shaderClass.hpp"
#include "render/gpu_interface/texture.hpp"
#include "render/renderObject.hpp"

RenderObject::~RenderObject()
{
    Cleanup();
}

void RenderObject::Cleanup()
{
    if (m_shader)
    {
        m_shader.reset();
    }
    for (auto& texture : m_textures)
    {
        if (get<0>(texture))
        {
            get<0>(texture).reset();
        }
    }
    if (m_vao)
    {
        m_vao.reset();
    }
    if (m_vbo)
    {
        m_vbo.reset();
    }
    if (m_ebo)
    {
        m_ebo.reset();
    }
}

void RenderObject::Render(const double_t& delta_time, const float_t& fov, const float_t& near, const float_t& far, Shader& used_shader)
{
    used_shader.Activate();
    
    glUniform1i(glGetUniformLocation(used_shader.GetID(), "shadowMap"), 0);

    if (used_shader.GetType() == ShaderType::Fun)
    {
        glm::vec2 size = glm::vec2(1200, 1000);
        glUniform2fv(glGetUniformLocation(used_shader.GetID(), "windowSize"), 1, glm::value_ptr(size));

        GLfloat dt = delta_time;
        glUniform1f(glGetUniformLocation(used_shader.GetID(), "deltaTime"), dt);
    }

    if (m_textures.size() > 0 && used_shader.GetType() != ShaderType::Shadow)
    {
        glUniform1i(glGetUniformLocation(used_shader.GetID(), "hasColorTexture"), m_has_color_texture ? 1 : 0);
        std::get<0>(m_textures[0])->Bind(GL_TEXTURE1);
        glUniform1i(glGetUniformLocation(used_shader.GetID(), "tex0"), 1);
    }

    m_vao->Bind();
    m_ebo->Bind();

    m_camera->Matrix(fov, near, far, used_shader, "view", "projection");

    for (const auto& light : m_lights)
    {
        light->ExportLight(used_shader);
    }

    glUniform3fv(glGetUniformLocation(used_shader.GetID(), "viewPos"), 1, glm::value_ptr(m_camera->GetPosition()));
    glUniformMatrix4fv(glGetUniformLocation(used_shader.GetID(), "model"), 1, GL_FALSE, glm::value_ptr(m_model_mat));
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);

    if (m_textures.size() > 0 && used_shader.GetType() != ShaderType::Shadow)
    {
        std::get<0>(m_textures[0])->Unbind();
    }
}

void RenderObject::Render(const double_t& delta_time, const float_t& fov, const float_t& near, const float_t& far)
{
    Render(delta_time, fov, near, far, *m_shader.get());
}

const glm::vec3 RenderObject::GetPos()
{
    return glm::vec3(m_model_mat[3][0], m_model_mat[3][1], m_model_mat[3][2]);;
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

void RenderObject::AddLight(const std::shared_ptr<Light>& light)
{
    m_lights.push_back(light);
}

void RenderObject::RemoveLight(const std::shared_ptr<Light>& light)
{
    std::erase(m_lights, light);
}

void RenderObject::SetShaderType(const ShaderType& type)
{
    m_shader_type = type;
}

void RenderObject::AddShader()
{
    m_shader = std::make_unique<Shader>(m_shader_type);
}

void RenderObject::AddTexture(std::unique_ptr<Texture> texture, const std::string& texUni)
{
    m_has_color_texture = true;
    m_textures.push_back(std::make_tuple(std::move(texture), texUni));
}

void RenderObject::ApplyTexture()
{
    if (!m_has_color_texture || m_textures.empty())
    {
        return;
    }

    m_vao->Bind();
    get<0>(m_textures.back())->TexUnit(*m_shader, get<1>(m_textures.back()).c_str(), 0);
}

const bool& RenderObject::IsConcave() const
{
    return m_is_concave;
}

const bool& RenderObject::GetVisible() const
{
    return m_is_visible;
}

void RenderObject::SetVisible(const bool& is_visible)
{
    m_is_visible = is_visible;
}

void RenderObject::PushToVerts(const std::initializer_list<float_t>& coords)
{
    for (const auto& coord : coords)
    {
        m_vertices.push_back(coord);
    }
}

void RenderObject::PushToInds(const std::initializer_list<uint64_t>& coords)
{
    for (const auto& coord : coords)
    {
        m_indices.push_back(coord);
    }
}