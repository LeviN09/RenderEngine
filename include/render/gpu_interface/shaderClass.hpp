#ifndef SHADER_CLASS_HPP
#define SHADER_CLASS_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>

std::string get_file_contents(const std::string& filename);

class Shader
{
    public:
        Shader(const std::string& vertexFile, const std::string& fragmentFile);

        const GLuint GetID() const;

        void Activate();
        void Delete();

    private:
        GLuint ID;

};

#endif