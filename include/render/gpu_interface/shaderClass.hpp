#ifndef SHADER_CLASS_HPP
#define SHADER_CLASS_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>
#include <unordered_map>

std::string get_file_contents(const std::string& filename);

enum class ShaderType {
    Default,
    Light,
    Fun,
    Shadow,
    Test
};

struct ShaderPath {
    const std::string vert_shader;
    const std::string frag_shader;
};

static const std::unordered_map<const ShaderType, const ShaderPath> s_shader_files =
{
    {
        ShaderType::Default, ShaderPath("../shaders/default.vert", "../shaders/default.frag")
    },
    {
        ShaderType::Light, ShaderPath("../shaders/light.vert", "../shaders/light.frag")
    },
    {
        ShaderType::Fun, ShaderPath("../shaders/fun.vert", "../shaders/fun.frag")
    },
    {
        ShaderType::Shadow, ShaderPath("../shaders/shadow.vert", "../shaders/shadow.frag")
    },
    {
        ShaderType::Test, ShaderPath("../shaders/default.vert", "../shaders/test.frag")
    }
};

class Shader
{
    public:
        Shader(const ShaderType type);

        const GLuint GetID() const;
        const ShaderType GetType() const;

        void Activate();
        void Delete();

    private:
        GLuint m_id;
        const ShaderType m_type;

};

#endif