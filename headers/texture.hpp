#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "../vendor/stb_image.h"

#include "shaderClass.hpp"

class Texture
{
public:
	GLuint ID;
	GLenum type;
	Texture(const char* image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType);

	void TexUnit(Shader& shader, const char* uniform, GLuint unit);
	void Bind(GLuint slot);
	void Unbind();
	void Delete();
};
#endif