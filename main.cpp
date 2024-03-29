#include <glm/ext/matrix_clip_space.hpp>
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

#include "headers/light.hpp"
#include "headers/physicsEngine.hpp"
#include "headers/renderer.hpp"
#include "headers/shaderClass.hpp"
#include "headers/VAO.hpp"
#include "headers/VBO.hpp"
#include "headers/EBO.hpp"
#include "headers/sphere.hpp"
#include "headers/texture.hpp"
#include "headers/camera.hpp"
#include "headers/cube.hpp"
#include "headers/plane.hpp"

#include "vendor/stb_image.h"

void error(const std::string& msg) {
    std::cerr << msg << std::endl;
    exit(-1);
}

void say(const std::string& msg) {
    std::cout << msg << std::endl;
}

int width = 800, height = 800;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    if (window != NULL)
        glViewport(0, 0, width, height);
}

int main() {

    if (!glfwInit()) { error("GLFW init"); }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(width, height, "Engine", NULL, NULL);
    if (window == NULL) {
        glfwTerminate();
        error("Window NULL");
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK) { error("GLEW init"); }

    glViewport(0, 0, width, height);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    Renderer renderer(window, width, height);
    renderer.AddCurrCamera();

    PhysicsEngine engine(&renderer);

    SphereBody* planet1 = new SphereBody(glm::vec3(-50.0f, 30.0f, 0.0f), 3, 10);
    SphereBody* planet2 = new SphereBody(glm::vec3(0.0f, 30.0f, 0.0f), 5, 1000);
    SphereBody* planet3 = new SphereBody(glm::vec3(-1.0f, 15.0f, 30.0f), 5, 1000);
    planet1->SetGravity(true);
    planet2->SetGravity(true);
    planet3->SetGravity(true);

    planet1->Push(glm::vec3(0.0f, 0.0f, -0.01f));

    engine.AddObject(planet1);
    engine.AddObject(planet2);
    engine.AddObject(planet3);

    Light* lighty = new Light();
    lighty->Translate(glm::vec3(0.0f, 0.5f, 0.0f));
    renderer.AddLight(lighty);


    Cube* cubey = new Cube(10.0f);
    cubey->Translate(glm::vec3(10.0f, 5.0f, -10.0f));
    renderer.AddObject(cubey);

    Cube* cubey2 = new Cube(3.0f);
    cubey2->Rotate(glm::radians(30.0f), glm::vec3(1.0f));
    cubey2->Translate(glm::vec3(15.0f));
    cubey2->AddTexture(new Texture("res/cirnofumo.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE), "tex0");
    renderer.AddObject(cubey2);

    Plane* planey = new Plane();
    planey->Rotate(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    planey->AddTexture(new Texture("res/cacopog.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE), "tex0");
    renderer.AddObject(planey);

    Plane* planey2 = new Plane();
    //planey->Rotate(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    planey2->Translate(glm::vec3(10.0f));
    renderer.AddObject(planey2);

    Sphere* spherey = new Sphere(3.0f, 10, 10);
    //renderer.AddObject(spherey);

    Sphere* s2 = new Sphere(10.0f, 40, 40);
    s2->Translate(glm::vec3(-15.f, 0.0f, 0.0f));
    renderer.AddObject(s2);

    double time = glfwGetTime();
    double prevTime = time;

    glEnable(GL_DEPTH_TEST);
    glDepthMask(true);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while (!glfwWindowShouldClose(window)) {
        time = glfwGetTime();

        glClearColor(0.0f, 0.0f, 0.0f, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glfwGetWindowSize(window, &width, &height);

        engine.Update();

        renderer.Update();
        s2->Rotate(glm::radians(1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        cubey->Rotate(glm::radians(1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        //lighty->Translate(glm::vec3(0.0f, 0.0f, 0.01f));
        renderer.Render();

        glfwSwapBuffers(window);

        glfwPollEvents();

        prevTime = time;
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}