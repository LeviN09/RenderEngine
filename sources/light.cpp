#include "../headers/light.hpp"
#include <glm/gtc/type_ptr.hpp>

void Light::InitLight(Shader* _shader) {
    shader = _shader;
    //std::cout << shader->ID << " " << position.x << " " << color.y << std::endl;
    glUniform3fv(glGetUniformLocation(shader->ID, "lightPos"), 1, glm::value_ptr(position));
    glUniform3fv(glGetUniformLocation(shader->ID, "lightColor"), 1, glm::value_ptr(color));
}

void Light::Translate(glm::vec3 translate) {
    position += translate;
}