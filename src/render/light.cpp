#include <glm/gtc/type_ptr.hpp>

#include "render/light.hpp"

void Light::ExportLight(const Shader& shader)
{
    //std::cout << shader->ID << " " << position.x << " " << color.y << std::endl;
    glUniform3fv(glGetUniformLocation(shader.GetID(), "lightPos"), 1, glm::value_ptr(position));
    glUniform3fv(glGetUniformLocation(shader.GetID(), "lightColor"), 1, glm::value_ptr(color));
}

void Light::Translate(const glm::vec3& translate)
{
    position += translate;
}