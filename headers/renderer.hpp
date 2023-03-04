#ifndef RENDERER_CLASS_HPP
#define RENDERER_CLASS_HPP

#include "physicsObject.hpp"
#include "renderObject.hpp"
#include "shaderClass.hpp"
#include "VAO.hpp"
#include "VBO.hpp"
#include "EBO.hpp"
#include "camera.hpp"
#include "light.hpp"

#include <GLFW/glfw3.h>
#include <vector>

class Renderer {
    private:
        Camera* currCam;

        GLFWwindow* window;
        int width, height;

        std::vector<Camera*> cameras;
        std::vector<RenderObject*> objects;
        std::vector<Light*> lights;

    public:
        Renderer(GLFWwindow* _window, const int& _width, const int& _height):
        window{_window}, width{_width}, height{_height} {}

        VAO* AddCurrVAO();
        VAO* AddVAO();
        Camera* AddCurrCamera();
        Camera* AddCamera();
        Light* AddLight(Light* light);
        void AddObject(RenderObject* object);
        void AddObject(PhysicsObject* object);
        Camera* GetCurrCam() { return currCam; }
        void SetCurrCam(Camera* cam);

        void Render();
        void Update();
};

#endif