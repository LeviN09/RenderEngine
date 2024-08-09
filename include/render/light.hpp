#pragma once

#include <glm/ext/vector_float3.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>

#include "render/gpu_interface/shaderClass.hpp"
#include "scene/idTag.hpp"

class Light : public IdTag
{
    public:
        ~Light() = default;

        void Translate(const glm::vec3& translate);
        void SetPosition(const glm::vec3& position);

        virtual void ExportLight(Shader& shader) = 0;
    
    protected:
        Light(const std::string& uid, const glm::vec3& position, const glm::vec3& color, const float_t& intentsity):
            IdTag(uid), m_position{ position }, m_color{ color }, m_intensity{ intentsity }
        {}

        glm::vec3 m_position{ 0.0f };
        glm::vec3 m_color{ 1.0f };
        float_t m_intensity{ 1.0f };
};

class DirectionalLight : public Light
{
    public:
        DirectionalLight(const std::string& uid, const glm::vec3& direction, const glm::vec3& color, const float_t& intentsity):
            Light(uid, glm::vec3{ 0.0f }, color, intentsity), m_shadow_shader{ std::make_unique<Shader>(ShaderType::Shadow) }
        {
            SetDirection(direction);
            InitShadowMap();
        }

        void RenderShadowMap();
        const GLuint& GetShadowMap();
        Shader& GetShadowShader();
        void ExportLight(Shader& shader) override;

        void SetDirection(const glm::vec3& direction);
        void Rotate(float_t rad, const glm::vec3& rotate);
        const glm::vec3& GetDirection() const;

    private:
        void InitShadowMap();
        void UpdateLightProjection();

        glm::vec3 m_direction;

        glm::mat4 m_orthogonal_projection;
        glm::mat4 m_light_view;
        glm::mat4 m_light_projection;

        GLuint m_shadowmap_width = 4096, m_shadowmap_height = 4096;
        const float_t m_projection_size{ 40.0f };

        GLuint m_shadowmap_fbo;
        GLuint m_shadowmap;
        std::unique_ptr<Shader> m_shadow_shader;
};

class PointLight : public Light
{
    public:
        PointLight(const std::string& uid, const glm::vec3& position, const glm::vec3& color, const float_t& intentsity):
            Light(uid, position, color, intentsity)
        {}
    
        void ExportLight(Shader& shader) override;
};