#ifndef VAO_CLASS_HPP
#define VAO_CLASS_HPP

#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "VBO.hpp"

class VAO {
    public:
        VAO();

        void LinkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);
        void Bind();
        void Unbind();
        void ActivateLayout(GLuint layout);
        void DisableLayout(GLuint layout);
        void Delete();

        GLuint GetID() const;

    private:
        GLuint m_id;

};

#endif