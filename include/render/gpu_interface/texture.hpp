#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "render/gpu_interface/shaderClass.hpp"

class Texture
{
	public:
		Texture(const std::string& image, GLenum tex_type, GLenum slot, GLenum format, GLenum pixel_type);

		const GLuint& GetID() const;
		const GLenum& GetType() const;
		const GLenum& GetSlot() const;

		void TexUnit(Shader& shader, const std::string& uniform, GLuint unit);
		void Bind(GLuint slot);
		void Unbind();
		void Delete();

	private:
		GLuint m_id;
		GLenum m_type;
		GLenum m_slot;
};
#endif