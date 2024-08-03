#ifndef LIGHT_CLASS_HPP
#define LIGHT_CLASS_HPP

#include <glm/ext/vector_float3.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "render/gpu_interface/shaderClass.hpp"
#include "scene/idTag.hpp"

class Light : public IdTag
{
    public:
        ~Light() = default;

        void Translate(const glm::vec3& translate);

        virtual void ExportLight(const Shader& shader) = 0;
    
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
            Light(uid, glm::vec3{ 0.0f }, color, intentsity)
        {
            SetDirection(direction);
        }

    void ExportLight(const Shader& shader) override;

    void SetDirection(const glm::vec3& direction);
    const glm::vec3& GetDirection() const;

    private:
        glm::vec3 m_direction;
};

class PointLight : public Light
{
    public:
        PointLight(const std::string& uid, const glm::vec3& position, const glm::vec3& color, const float_t& intentsity):
            Light(uid, position, color, intentsity)
        {}
    
    void ExportLight(const Shader& shader) override;
};

#endif