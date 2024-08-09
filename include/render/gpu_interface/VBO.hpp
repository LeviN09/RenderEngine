#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class VBO
{
    public:
        VBO(GLfloat* vertices, GLsizeiptr size);

        void Bind();
        void Unbind();
        void Delete();
        
        GLuint GetID() const;

    private:
        GLuint m_id;
};