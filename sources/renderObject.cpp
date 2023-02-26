#include "../headers/renderObject.hpp"

#include "../headers/VBO.hpp"
#include "../headers/VAO.hpp"
#include "../headers/EBO.hpp"

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <tuple>

RenderObject::~RenderObject() {
    delete vao;
    delete vbo;
    delete ebo;
    delete shader;
    for (auto& tex : textures) { delete std::get<0>(tex); }
}

void RenderObject::Render(const float &fov, const float &near, const float &far) {
    shader->Activate();
    if (textures.size() > 0) {
        glUniform1i(glGetUniformLocation(shader->ID, "hasColorTexture"), hasColorTexture ? 1 : 0);
        std::get<0>(textures[0])->Bind(GL_TEXTURE0);
    }
    vao->Bind();
    ebo->Bind();
    camera->Matrix(fov, near, far, *shader, "camMatrix");
    light->InitLight(shader);
    //std::cout << "vao " << vao->ID << " vbo " << vbo->ID << " ebo " << ebo->ID << " shader " << shader->ID << std::endl;
    glUniformMatrix4fv(glGetUniformLocation(shader->ID, "model"), 1, GL_FALSE, glm::value_ptr(modelMat));
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    if (textures.size() > 0) {
        std::get<0>(textures[0])->Unbind();
    }
}

void RenderObject::ConfigureVBO() {
    GLfloat* tvbo = &vertices[0];

    vbo = new VBO(tvbo, vertices.size() * sizeof(GLfloat));

}

void RenderObject::ConfigureEBO() {
    GLuint* tebo = &indices[0];

    ebo = new EBO(tebo, indices.size() * sizeof(GLuint));

}

void RenderObject::ConfigureVAO(std::vector<VAOInfo> attrs) {
    vao = new VAO();
    vao->Bind();
    for (auto& attr : attrs) {
        vao->LinkAttrib(*vbo, attr.layout, attr.numComponents, attr.type, attr.stride, (void*)attr.offset);
    }
}

void RenderObject::Configure() {
    ConfigureVBO();
    ConfigureVAO(VAOInfo::GetVAOInfo(hasColor, hasNormals, hasColorTexture));
    ConfigureEBO();
}

void RenderObject::AddShader() {
    shader = new Shader("shaders/default.vert", "shaders/default.frag");
}

void RenderObject::AddTexture(Texture *texture, std::string texUni) {
    vao->Bind();
    hasColorTexture = true;
    textures.push_back(std::make_tuple(texture, texUni));
    texture->TexUnit(*shader, texUni.c_str(), 0);
}

void RenderObject::Translate(glm::vec3 transform) {
    modelMat = glm::translate(modelMat, transform);
}

void RenderObject::Rotate(float rad, glm::vec3 rotate) {
    modelMat = glm::rotate(modelMat, rad, rotate);
}

void RenderObject::Scale(glm::vec3 scale) {
    modelMat = glm::scale(modelMat, scale);
}

void RenderObject::Scale(float scale) {
    Scale(glm::vec3(scale, scale, scale));
}

void RenderObject::PushToVerts(std::initializer_list<float> coords) {
    for (auto& coord : coords) {
        vertices.push_back(coord);
    }
}

void RenderObject::PushToInds(std::initializer_list<int> coords) {
    for (auto& coord : coords) {
        indices.push_back(coord);
    }
}