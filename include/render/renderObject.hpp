#ifndef RENDEROBJECT_CLASS_HPP
#define RENDEROBJECT_CLASS_HPP

#include <GL/glew.h>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <initializer_list>
#include <memory>
#include <string>
#include <vector>

#include "render/camera.hpp"
#include "render/gpu_interface/EBO.hpp"
#include "render/gpu_interface/shaderClass.hpp"
#include "render/gpu_interface/texture.hpp"
#include "render/gpu_interface/VAO.hpp"
#include "render/gpu_interface/VBO.hpp"
#include "render/light.hpp"
#include "scene/idTag.hpp"

class VAOInfo
{
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
        GetVAOInfo(bool hasColor, bool hasNormals, bool hasColorTexture)
        {
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

            if (hasNormals)
            {
                offset += prevOffset;
                prevOffset = sizeofNormals;
                //++layout;
                attrs.push_back(VAOInfo(1, 3, GL_FLOAT, fullSize, offset));
            }

            if (hasColor)
            {
                offset += prevOffset;
                prevOffset = sizeofColor;
                //++layout;
                attrs.push_back(VAOInfo(2, 3, GL_FLOAT, fullSize, offset));
            }


            if (hasColorTexture)
            {
                offset += prevOffset;
                prevOffset = sizeofColorTexture;
                //++layout;
                attrs.push_back(VAOInfo(3, 2, GL_FLOAT, fullSize, offset));
            }

            return attrs;
        }


};

class RenderObject : public IdTag
{
    public:
        ~RenderObject() = default;

        void Render(const double_t& delta_time, const float_t& fov, const float_t& near, const float_t& far);

        virtual void Init() = 0;
        virtual void Update(const double_t& delta_time) = 0;

        
        const VAO& GetVAO();
        const VBO& GetVBO();

        void Configure();
        void AddShader(const ShaderType& type);
        void SetCamera(const std::shared_ptr<Camera>& camera);
        void SetLight(const std::shared_ptr<Light>& light);
        void AddTexture(std::unique_ptr<Texture> texture, const std::string& tex_uni);
        
        void PushToVerts(const std::initializer_list<float_t>& coords);
        void PushToInds(const std::initializer_list<int64_t>& coords);

    protected:
        RenderObject(const std::string& uid, const glm::mat4& model_mat):
            IdTag(uid), m_model_mat(model_mat)
        {}

        std::vector<GLfloat> m_vertices;
        std::vector<GLuint> m_indices;
        const glm::mat4& m_model_mat;

        std::unique_ptr<VAO> m_vao;
        std::unique_ptr<VBO> m_vbo;
        std::unique_ptr<EBO> m_ebo;

        std::shared_ptr<Camera> m_camera;
        std::shared_ptr<Light> m_light;
        std::unique_ptr<Shader> m_shader;
        std::vector<std::tuple<std::unique_ptr<Texture>, std::string>> m_textures;

        bool m_has_color{ true }, m_has_normals, m_has_color_texture{ false };

        //bool isVisible{true};
        //bool isLocked{false};

    private:
        void ConfigureVAO(const std::vector<VAOInfo>& attrs);
        void ConfigureVBO();
        void ConfigureEBO();
};

#endif