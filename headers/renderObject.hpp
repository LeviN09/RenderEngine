#ifndef RENDEROBJECT_CLASS_HPP
#define RENDEROBJECT_CLASS_HPP

#include "VAO.hpp"
#include "VBO.hpp"
#include "EBO.hpp"
#include "texture.hpp"
#include "shaderClass.hpp"
#include "camera.hpp"

#include <GL/glew.h>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <initializer_list>
#include <utility>
#include <memory>
#include <vector>
#include <string>

class VAOInfo {
    public:
        GLuint layout, numComponents;
        GLenum type;
        GLsizeiptr stride;
        GLsizeiptr offset;

        VAOInfo(GLuint _layout, GLuint _numComponents,
        GLenum _type, GLsizeiptr _stride, GLsizeiptr _offset):
        layout(_layout), numComponents(_numComponents), type(_type),
        stride(_stride), offset(_offset) {}

        static std::vector<VAOInfo>
        GetVAOInfo(bool hasColor, bool hasNormals, bool hasColorTexture) {
            std::vector<VAOInfo> attrs;

            unsigned int sizeofPos{3 * sizeof(float)};
            unsigned int sizeofColor{3 * sizeof(float)};
            unsigned int sizeofNormals{3 * sizeof(float)};
            unsigned int sizeofColorTexture{2 * sizeof(float)};

            unsigned int fullSize{sizeofPos};
            unsigned int offset{0};
            //unsigned int layout{0};

            if (hasColor) { fullSize += sizeofColor; }
            if (hasNormals) { fullSize += sizeofNormals; }
            if (hasColorTexture) { fullSize += sizeofColorTexture; }

            attrs.push_back(VAOInfo(0, 3, GL_FLOAT, fullSize, offset));
            unsigned int prevOffset{sizeofPos};

            if (hasNormals) {
                offset += prevOffset;
                prevOffset = sizeofNormals;
                //++layout;
                attrs.push_back(VAOInfo(1, 3, GL_FLOAT, fullSize, offset));
            }

            if (hasColor) {
                offset += prevOffset;
                prevOffset = sizeofColor;
                //++layout;
                attrs.push_back(VAOInfo(2, 3, GL_FLOAT, fullSize, offset));
            }


            if (hasColorTexture) {
                offset += prevOffset;
                prevOffset = sizeofColorTexture;
                //++layout;
                attrs.push_back(VAOInfo(3, 2, GL_FLOAT, fullSize, offset));
            }

            return attrs;
        }


};

class RenderObject {
    protected:
        std::vector<GLfloat> vertices;
        std::vector<GLuint> indices;
        Shader* shader;
        Camera* camera;
        VAO* vao;
        VBO* vbo;
        EBO* ebo;
        std::vector<std::tuple<Texture*, std::string>> textures;
        bool hasColor{true}, hasNormals, hasColorTexture{false};

        glm::mat4 modelMat;

        //RenderObject* parent{nullptr};
        //std::vector<RenderObject*> children;
        //bool isVisible{true};
        //bool isLocked{false};

    public:
        RenderObject(): modelMat{1.0f} {}
        ~RenderObject();

        void Render(const float& fov, const float& near, const float& far);
        virtual void Update() = 0;

        VAO* GetVAO() { return vao; }
        VBO* GetVBO() { return vbo; }

        //void AddChildren(std::initializer_list<RenderObject*>);

        void Configure();
        void AddShader();
        void SetCamera(Camera* _camera) { camera = _camera; }
        void AddTexture(Texture* texture, std::string texUni);
        
        void Translate(glm::vec3 transform);
        void Rotate(float rad, glm::vec3 rotate);
        void Scale(float scale);
        void Scale(glm::vec3 scale);
        void PushToVerts(std::initializer_list<float> coords);
        void PushToInds(std::initializer_list<int> coords);

    private:
        void ConfigureVAO(std::vector<VAOInfo> attrs);
        void ConfigureVBO();
        void ConfigureEBO();
};

#endif