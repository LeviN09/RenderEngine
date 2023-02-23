#ifndef LIGHT_CLASS_HPP
#define LIGHT_CLASS_HPP

#include <glm/ext/vector_float3.hpp>

class Light {
    private:
        glm::vec3 position;
        glm::vec3 color;
        Light();
        ~Light();

    public:
};

class PointLight : public Light {

};

#endif