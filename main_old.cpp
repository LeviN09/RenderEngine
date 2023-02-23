/*#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include <glm/trigonometric.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "headers/shaderClass.hpp"
#include "headers/VAO.hpp"
#include "headers/VBO.hpp"
#include "headers/EBO.hpp"
#include "headers/texture.hpp"
#include "headers/camera.hpp"

#include "vendor/stb_image.h"

void error(std::string msg) {
    std::cerr << msg << std::endl;
    exit(-1);
}

int width = 800, height = 800;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    if (window != NULL)
        glViewport(0, 0, width, height);
}

int asdmain() {

    if (!glfwInit()) { error("GLFW init"); }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLfloat vertices[] = {
        -0.5f    , -0.5f * float(sqrt(3)) / 3    , 0.0f, 0.8f, 0.8f, 0.7f,
         0.5f    , -0.5f * float(sqrt(3)) / 3    , 0.0f, 0.8f, 0.8f, 0.3f,
         0.0f    ,  0.5f * float(sqrt(3)) * 2 / 3, 0.0f, 1.0f, 0.8f, 0.7f,
        -0.5f / 2,  0.5f * float(sqrt(3)) / 6    , 0.0f, 0.9f, 0.5f, 0.3f,
         0.5f / 2,  0.5f * float(sqrt(3)) / 6    , 0.0f, 0.9f, 0.5f, 0.7f,
         0.0f    , -0.5f * float(sqrt(3)) / 3    , 0.0f, 0.8f, 0.5f, 0.7f,
    };

    GLuint indices[] = {
        0, 3, 5,
        3, 2, 4,
        5, 4, 1
    };

    GLfloat vertices[] = {
        -0.5f , -0.5f , -0.5f,   1.0f , 0.5f , 1.0f,      0.0f , 0.0f,
        -0.5f ,  0.5f ,  0.5f,   1.0f , 0.5f , 0.0f,      0.0f , 1.0f,
         0.5f ,  0.5f , -0.5f,   1.0f , 0.5f , 0.0f,      1.0f , 1.0f,
         0.5f , -0.5f ,  0.5f,   1.0f , 0.5f , 1.0f,      1.0f , 0.0f
    };

    GLuint indices[] = {
        0, 2, 1,
        0, 3, 2,
        0, 3, 1,
        3, 1, 2
    };

    GLFWwindow* window = glfwCreateWindow(width, height, "Hola", NULL, NULL);
    if (window == NULL) {
        glfwTerminate();
        error("Window NULL");
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK) { error("GLEW init"); }

    glViewport(0, 0, width, height);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


    Shader shaderProgram("shaders/default.vert", "shaders/default.frag");

    VAO VAO1;
    VAO1.Bind();

    VBO VBO1(vertices, sizeof(vertices));
    EBO EBO1(indices, sizeof(indices));

    VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
    VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();

    //GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

    GLuint modelID = glGetUniformLocation(shaderProgram.ID, "model");
    GLuint winSizeID = glGetUniformLocation(shaderProgram.ID, "windowSize");

    Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

    Texture fumo("res/cirnofumo.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
    fumo.texUnit(shaderProgram, "tex0", 0);

    double prevTime = glfwGetTime();

    glm::mat4 model;

    glEnable(GL_DEPTH_TEST);
    glDepthMask(true);

    fumo.Bind(GL_TEXTURE0);
    VAO1.Bind();

    int prevPress = GLFW_PRESS;

    while (!glfwWindowShouldClose(window)) {

        glClearColor(0.5f, 0.5f, 0.6f, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glfwGetWindowSize(window, &width, &height);
        glUniform2f(winSizeID, width, height);
        //glUseProgram(shaderProgram.ID);
        shaderProgram.Activate();

        double time = glfwGetTime();

        model = glm::mat4(1.0f);

        camera.Inputs(window);
        camera.Matrix(45.0f, 0.1f, 100.0f, shaderProgram, "camMatrix");

        glUniformMatrix4fv(modelID, 1, GL_FALSE, glm::value_ptr(model));

        //glUniform1f(uniID, scale);

        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(GLuint), GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();
    fumo.Delete();
    //glDeleteProgram(shaderProgram);
    shaderProgram.Delete();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}*/