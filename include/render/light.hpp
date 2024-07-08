#ifndef LIGHT_CLASS_HPP
#define LIGHT_CLASS_HPP

#include <glm/ext/vector_float3.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "render/gpu_interface/shaderClass.hpp"
#include "scene/idTag.hpp"

class Light : public IdTag
{
    public:
        Light() = default;
        ~Light() = default;
        void ExportLight(const Shader& shader);
        void Translate(const glm::vec3& translate);
    
    protected:
        glm::vec3 position{ glm::vec3(0.0f, 0.0f, 0.0f) };
        glm::vec3 color{ glm::vec3(1.0f, 1.0f, 1.0f) };
};

/*class PointLight : public Light {
    public:
        PointLight(): Light() {
            position = glm::vec3(1.0f);
            color = glm::vec3(1.0f);
        }
};*/

#endif