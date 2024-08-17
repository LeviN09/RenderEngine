#define GLM_ENABLE_EXPERIMENTAL
#include <chrono>
#include <GL/glew.h>
#include <GL/glext.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/trigonometric.hpp>
#include <memory>
#include <string>
#include <unistd.h>

#include "physics/physicsEngine.hpp"
#include "render/gpu_interface/shaderClass.hpp"
#include "render/gpu_interface/texture.hpp"
#include "render/light.hpp"
#include "render/renderer.hpp"
#include "scene/generators/heightmapGenerator.hpp"
#include "scene/generators/perlinNoiseGenerator.hpp"
#include "scene/primitives/cube.hpp"
#include "scene/primitives/object_builders/cubeParts.hpp"
#include "scene/primitives/object_builders/planeParts.hpp"
#include "scene/primitives/object_builders/sphereParts.hpp"
#include "scene/primitives/heightmap.hpp"
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

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
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
    renderer.AddLight(std::make_shared<DirectionalLight>("dirLight", glm::vec3(1.0f), glm::vec3(0.7f, 0.9f, 1.0f), 1.0f));
    //renderer.AddLight(std::make_shared<PointLight>("pointLight", glm::vec3(-15.0f, 15.0f, 15.0f), glm::vec3(1.0f, 1.0f, 0.0f), 1.0f));

    PhysicsEngine engine;

    SphereObject planet1(renderer, engine, "planet1", glm::vec3(5.0f), 3.0f, 10.0f);
    planet1.AddRenderObject(ShaderType::Fun);
    planet1.AddPhysicsObject();

    SphereObject planet2(renderer, engine, "planet2", glm::vec3(-2.0f), 2.5f, 20.0f);
    planet2.AddRenderObject(ShaderType::Light);
    planet2.AddPhysicsObject();

    SphereObject planet3(renderer, engine, "planet3", glm::vec3(-5.0f, 0.0f, 5.0f), 2.0f, 5.0f);
    planet3.AddRenderObject(ShaderType::Cellshade);
    planet3.AddPhysicsObject();

    engine.GetObject<SphereBody>("p_planet2").Push(glm::vec3(0.02f));

    CubeObject cubey1(renderer, engine, "cubey1", glm::vec3(5.0f), 1.0f);
    cubey1.AddRenderObject();

    std::unique_ptr<Texture> cirno = std::make_unique<Texture>(res_prefix + "textures/cirnofumo.png", GL_TEXTURE_2D, GL_TEXTURE1, GL_RGB, GL_UNSIGNED_BYTE);
    std::unique_ptr<Texture> caco = std::make_unique<Texture>(res_prefix + "textures/cacopog.png", GL_TEXTURE_2D, GL_TEXTURE1, GL_RGB, GL_UNSIGNED_BYTE);

    renderer.GetObject<CubeRender>("r_cubey1").AddTexture(std::move(cirno), "tex0");

    PlaneObject planey(renderer, engine, "planey1", glm::vec3(-5.0f), 10.0f);
    planey.AddRenderObject();

    renderer.GetObject<PlaneRender>("r_planey1").AddTexture(std::move(caco), "tex0");

    PlaneObject depthMapMap(renderer, engine, "depthMapMap", glm::vec3(-5.0f, 3.0f, 0.0f), 5.0f);
    depthMapMap.AddRenderObject(ShaderType::Test);

    SphereObject testS(renderer, engine, "testS", glm::vec3(0.0f), 0.5f);
    testS.AddRenderObject();

    PlaneObject axis1(renderer, engine, "axis1", glm::vec3(0.0f), 1.0f);
    axis1.AddRenderObject();

    PlaneObject axis2(renderer, engine, "axis2", glm::vec3(0.0f), 1.0f);
    axis2.AddRenderObject();

    axis2.Rotate(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    PlaneObject axis3(renderer, engine, "axis3", glm::vec3(0.0f), 1.0f);
    axis3.AddRenderObject();

    axis3.Rotate(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    axis3.Rotate(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    CubeObject brick(renderer, engine, "brick1", glm::vec3(10.0f, 0.0f, 5.0f), glm::vec3(0.75f, 1.25f, 1.75f));
    brick.AddRenderObject(ShaderType::Fun);
    brick.AddPhysicsObject();

    CubeObject brick2(renderer, engine, "brick2", glm::vec3(5.0f, -12.0f, -10.0f), glm::vec3(1.0f));
    brick2.AddRenderObject(ShaderType::Default);
    brick2.AddPhysicsObject();

/*     SphereObject earth(renderer, engine, "earth1", glm::vec3(0.0f, -210.0f, 0.0f), 200.0f, 10000000.0f);
    earth.AddRenderObject(ShaderType::Default);
    earth.AddPhysicsObject();

    engine.GetObject<SphereBody>("p_earth1").SetUniversalGravity(false); */

    CubeObject ground1(renderer, engine, "ground1", glm::vec3(0.0f, -15.0f, 0.0f), glm::vec3(100.0f, 1.0f, 100.0f), 1000000.0f);
    ground1.AddRenderObject();
    ground1.AddPhysicsObject();

    PerlinNoise perlin(1001);

    std::function<float_t(float_t, float_t)> test_map = [&](float_t i, float_t j)
    {
        return 250 * perlin.OcataveNoise(i / 2000.0f, j / 2000.0f, 12, 0.5f) - 50.0f;
        //return 10 * perlin.ocataveNoise(i / 100.0f, j / 100.0f, 8, 0.55f);
        //return 1 / i + 1 / j;
        //return -(i * i + j * j) / 20.0f;
        //return tan(static_cast<float_t>(i) / j);
        //return (j*sin(i / 1.0f) + i*cos(j / 1.0f)) / 100.0f;
        //return ((i - 6) * (i - 6) + (j - 6) * (j - 6)) / 10.0f;
    };

    HeightmapObject map1(renderer, engine, "map1", glm::vec3(-10.0f, -5.0f, 10.0f), 20.0f, 50, test_map);
    map1.AddRenderObject(ShaderType::Cellshade);

    HeightmapGenerator gen(renderer, engine, renderer.GetCurrCam(), test_map, glm::ivec2(5));

    engine.GetObject<CubeBody>("p_ground1").SetUniversalGravity(false);
    engine.GetObject<CubeBody>("p_ground1").SetNormalForce(false);

    double_t time = glfwGetTime();
    double_t prevTime = time;

    const uint64_t updateInterval = 1000;
    uint64_t frameCount = 0;
    auto lastTime = std::chrono::high_resolution_clock::now();
    auto fpsLastUpdate = lastTime;
    
    float_t accumulator{ 0.0f };
    float_t lastTimeFloat{ 0.0f };
    float_t fixedDeltaTime = 1.0f / 60.0f;

    glEnable(GL_DEPTH_TEST);
    glDepthMask(true);

    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(1.0f, 1.0f);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    gen.StartUpdate();
    while (!glfwWindowShouldClose(window))
    {
        auto currentTime = std::chrono::high_resolution_clock::now();
        auto deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastTime).count();
        lastTimeFloat = std::chrono::duration<float_t>(currentTime - lastTime).count();
        lastTime = currentTime;


        if (lastTimeFloat > 0.25f)
        {
            lastTimeFloat = 0.25f;
        }

        accumulator += lastTimeFloat;

        time = glfwGetTime();

        glClearColor(0.7f, 0.9f, 1.0f, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glfwGetWindowSize(window, &width, &height);

        if (accumulator >= fixedDeltaTime)
        {
            engine.Update(fixedDeltaTime);
            accumulator -= fixedDeltaTime;
        }

        renderer.Update(time, mouse_pos_x, mouse_pos_y);
        
        cubey1.Rotate(glm::radians(1.0f), glm::vec3(1.0f, 1.0f, -1.0f));
        planey.Rotate(glm::radians(1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        //brick2.Rotate(glm::radians(1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        //brick2.Translate(glm::vec3(0.0f, -0.001f, 0.0f));
        map1.Rotate(glm::radians(0.5f), glm::vec3(0.0f, 1.0f, 0.0f));

        //dynamic_cast<DirectionalLight*>(&renderer.GetLight("dirLight"))->Rotate(glm::radians(0.1f), glm::vec3(1.0f, 0.0f, -1.0f));
        dynamic_cast<DirectionalLight*>(&renderer.GetLight("dirLight"))->SetPosition(renderer.GetCurrCam().GetPosition());

        renderer.UpdateQueues();
        renderer.Render(time);

        glfwSwapBuffers(window);

        glfwPollEvents();

        prevTime = time;

        frameCount++;
        auto timeSinceLastUpdate = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - fpsLastUpdate).count();
        if (timeSinceLastUpdate >= updateInterval)
        {
            float_t fps = frameCount * 1000.0f / timeSinceLastUpdate;
            //std::cout << std::fixed << std::setprecision(1) << "FPS: " << fps << std::endl;
            frameCount = 0;
            fpsLastUpdate = currentTime;
        }
    }
    gen.EndUpdate();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}