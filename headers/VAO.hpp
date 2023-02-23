#ifndef VAO_CLASS_HPP
#define VAO_CLASS_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "VBO.hpp"

class VAO {
    public:
        GLuint ID;
        VAO();

        void LinkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);
        void Bind();
        void Unbind();
        void ActivateLayout(GLuint layout);
        void DisableLayout(GLuint layout);
        void Delete();

};

#endif