#ifndef LIGHT_CLASS_HPP
#define LIGHT_CLASS_HPP

#include "shaderClass.hpp"
#include <glm/ext/vector_float3.hpp>
#include <glm/gtc/type_ptr.hpp>

class Light {
    protected:
        glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
        Shader* shader;
    public:
        Light() {}
        ~Light();
        void InitLight(Shader* _shader);
        void Translate(glm::vec3 translate);

};

/*class PointLight : public Light {
    public:
        PointLight(): Light() {
            position = glm::vec3(1.0f);
            color = glm::vec3(1.0f);
        }
};*/

#endif