#ifndef CAMERA_CLASS_HPP
#define CAMERA_CLASS_HPP

#define GLM_ENABLE_EXPERIMENTAL

//Include GLEW before GL
#include <GL/glew.h>

#include <cstdint>
#include <cmath>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "render/gpu_interface/shaderClass.hpp"
#include "scene/idTag.hpp"

class Camera : public IdTag
{
    public:
	    Camera(const int64_t& width, const int64_t& height, const glm::vec3& position);
		Camera(const std::string& uid, const int64_t& width, const int64_t& height, const glm::vec3& position);

		const glm::vec3& GetPosition() const;
		const glm::vec3& GetOrientation() const;
		const glm::vec3& GetUp() const;

		const bool& IsFirstClick() const;
		const int64_t& GetWidth() const;
		const int64_t& GetHeight() const;

		const float_t& GetSpeed() const;
		const float_t& GetSensitivity() const;

	    void Matrix(const float_t& fov_deg, const float_t& near_plane, const float_t& far_plane, const Shader& shader, const std::string& view_uniform, const std::string& projection_uniform);
	    void Inputs(GLFWwindow& window, const double_t& xpos, const double_t& ypos);

	private:
		glm::vec3 m_position;
	    glm::vec3 m_orientation{ glm::vec3(0.0f, 0.0f, -1.0f) };
	    glm::vec3 m_up{ glm::vec3(0.0f, 1.0f, 0.0f) };

	    bool m_first_click{ true };
	    int64_t m_width{ 0 }, m_height { 0 };

	    float_t m_speed{ 0.1f };
	    float_t m_sensitivity{ 100.0f };
};
#endif