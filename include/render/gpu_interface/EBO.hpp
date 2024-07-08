#ifndef EBO_CLASS_HPP
#define EBO_CLASS_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class EBO {
    public:
        EBO(GLuint* indices, GLsizeiptr size);

        void Bind();
        void Unbind();
        void Delete();

        GLuint GetID() const;

    private:
        GLuint m_id;
};

#endif