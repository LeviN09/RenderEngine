#define GLM_ENABLE_EXPERIMENTAL
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/trigonometric.hpp>
#include <memory>
#include <string>

#include "physics/physicsEngine.hpp"
#include "render/renderer.hpp"
#include "render/gpu_interface/texture.hpp"
#include "scene/primitives/cube.hpp"
#include "scene/primitives/object_builders/cubeParts.hpp"
#include "scene/primitives/object_builders/planeParts.hpp"
#include "scene/primitives/object_builders/sphereParts.hpp"
#include "scene/primitives/plane.hpp"
#include "scene/primitives/sphere.hpp"

#include "stb_image.h"

#include <iostream>

void error(const std::string& msg)
{
    std::cerr << msg << std::endl;
    exit(-1);
}

void say(const std::string& msg)
{
    std::cout << msg << std::endl;
}

static int width = 1200, height = 1000;
static const std::string res_prefix{ "../res/" };

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    if (window != NULL)
    {
        glViewport(0, 0, width, height);
    }
}


static double mouse_pos_x, mouse_pos_y;

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    mouse_pos_x = xpos;
    mouse_pos_y = ypos;
    //std::cout << "Cursor Position: (" << xpos << ", " << ypos << ")" << std::endl;
}

int main()
{
    if (!glfwInit()) { error("GLFW init"); }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(width, height, "Engine", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        error("Window NULL");
    }

    glfwSetCursorPosCallback(window, cursor_position_callback);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK) { error("GLEW init"); }

    glViewport(0, 0, width, height);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    Renderer renderer(*window, width, height);
    renderer.AddCurrCamera("cam1");
    renderer.AddLight(std::make_shared<Light>());

    PhysicsEngine engine;

    SphereObject planet1(renderer, engine, "planet1", glm::vec3(5.0f), 3.0f);
    planet1.AddRenderObject("../shaders/default.vert", "../shaders/default.frag");
    planet1.AddPhysicsObject();

    SphereObject planet2(renderer, engine, "planet2", glm::vec3(0.0f), 3.0f);
    planet2.AddRenderObject("../shaders/light.vert", "../shaders/light.frag");
    planet2.AddPhysicsObject();

    engine.GetObject<SphereBody>("p_planet2").Push(glm::vec3(-0.01f));

    CubeObject cubey1(renderer, engine, "cubey1", glm::vec3(5.0f), 1.0f);
    cubey1.AddRenderObject();

    std::unique_ptr<Texture> cirno = std::make_unique<Texture>(res_prefix + "textures/cirnofumo.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
    std::unique_ptr<Texture> caco = std::make_unique<Texture>(res_prefix + "textures/cacopog.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);

    renderer.GetObject<CubeRender>("r_cubey1").AddTexture(std::move(cirno), "tex0");

    PlaneObject planey(renderer, engine, "planey1", glm::vec3(-5.0f), 1.0f);
    planey.AddRenderObject();

    renderer.GetObject<PlaneRender>("r_planey1").AddTexture(std::move(caco), "tex0");

    double time = glfwGetTime();
    double prevTime = time;

    glEnable(GL_DEPTH_TEST);
    glDepthMask(true);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while (!glfwWindowShouldClose(window))
    {
        time = glfwGetTime();

        glClearColor(0.3f, 0.15f, 0.0f, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glfwGetWindowSize(window, &width, &height);

        engine.Update();

        renderer.Update(mouse_pos_x, mouse_pos_y);
        renderer.GetLight().Translate(glm::vec3(0.001f, 0.0f, -0.01f));
        
        cubey1.Rotate(glm::radians(1.0f), glm::vec3(-0.01f));
        planey.Rotate(glm::radians(1.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        renderer.Render();

        glfwSwapBuffers(window);

        glfwPollEvents();

        prevTime = time;
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}