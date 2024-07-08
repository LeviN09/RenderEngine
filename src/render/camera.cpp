#include <cmath>
#include <cstdint>
#include <iostream>

#include "render/camera.hpp"
#include "scene/idTag.hpp"

Camera::Camera(const int64_t& width, const int64_t& height, const glm::vec3& m_position):
	m_width{ width }, m_height{ height }, m_position{ m_position }
{}

Camera::Camera(const std::string& uid, const int64_t& width, const int64_t& height, const glm::vec3& m_position):
	IdTag(uid), m_width{ width }, m_height{ height }, m_position{ m_position }
{}

const glm::vec3& Camera::GetPosition() const
{
	return m_position;
}

const glm::vec3& Camera::GetOrientation() const
{
	return m_orientation;
}

const glm::vec3& Camera::GetUp() const
{
	return m_up;
}

const bool& Camera::IsFirstClick() const
{
	return m_first_click;
}

const int64_t& Camera::GetWidth() const
{
	return m_width;
}

const int64_t& Camera::GetHeight() const
{
	return m_height;
}

const float_t& Camera::GetSpeed() const
{
	return m_speed;
}

const float_t& Camera::GetSensitivity() const
{
	return m_sensitivity;
}

void Camera::Matrix(const float_t& fov_deg, const float_t& near_plane, const float_t& far_plane, const Shader& shader, const std::string& uniform)
{
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	view = glm::lookAt(m_position, m_position + m_orientation, m_up);
	projection = glm::perspective(glm::radians(fov_deg), (float)m_width / m_height, near_plane, far_plane);

	//std::cout << "coords " << m_position.x << " " << m_position.y << " " << m_position.z << std::endl;
	glUniformMatrix4fv(glGetUniformLocation(shader.GetID(), uniform.c_str()), 1, GL_FALSE, glm::value_ptr(projection * view));
}

void Camera::Inputs(GLFWwindow& window, const double_t& xpos, const double_t& ypos)
{
	if (glfwGetKey(&window, GLFW_KEY_W) == GLFW_PRESS)
	{
		m_position += m_speed * m_orientation;
	}
	if (glfwGetKey(&window, GLFW_KEY_A) == GLFW_PRESS)
	{
		m_position += m_speed * -glm::normalize(glm::cross(m_orientation, m_up));
	}
	if (glfwGetKey(&window, GLFW_KEY_S) == GLFW_PRESS)
	{
		m_position += m_speed * -m_orientation;
	}
	if (glfwGetKey(&window, GLFW_KEY_D) == GLFW_PRESS)
	{
		m_position += m_speed * glm::normalize(glm::cross(m_orientation, m_up));
	}
	if (glfwGetKey(&window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		m_position += m_speed * m_up;
	}
	if (glfwGetKey(&window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		m_position += m_speed * -m_up;
	}
	if (glfwGetKey(&window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		m_speed = 0.4f;
	}
	else if (glfwGetKey(&window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
	{
		m_speed = 0.1f;
	}

	if (glfwGetMouseButton(&window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
    	glfwSetInputMode(&window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		static double last_xpos{ xpos }, last_ypos{ ypos };

		double x_diff = xpos - last_xpos;
		double y_diff = ypos - last_ypos;
		
		if (m_first_click)
		{
			last_xpos = xpos;
			last_ypos = ypos;
			m_first_click = false;
			return;;
		}

		float rot_x = m_sensitivity * y_diff / m_height;
		float rot_y = m_sensitivity * x_diff / m_width;

		glm::vec3 newm_orientation = glm::rotate(m_orientation, glm::radians(-rot_x), glm::normalize(glm::cross(m_orientation, m_up)));

		if (glm::abs(glm::angle(newm_orientation, m_up) - glm::radians(90.0f)) <= glm::radians(85.0f))
		{
			m_orientation = newm_orientation;
		}

		m_orientation = glm::rotate(m_orientation, glm::radians(-rot_y), m_up);

		last_xpos = xpos;
		last_ypos = ypos;
	}
	else if (glfwGetMouseButton(&window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
    	glfwSetInputMode(&window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		m_first_click = true;
	}
}