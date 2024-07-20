#include <string>

#include "render/gpu_interface/texture.hpp"

#include "stb_image.h"

Texture::Texture(const std::string& image, GLenum tex_type, GLenum slot, GLenum format, GLenum pixel_type)
{
	m_type = tex_type;

	int widthImg, heightImg, numColCh;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* bytes = stbi_load(image.c_str(), &widthImg, &heightImg, &numColCh, 0);

	glGenTextures(1, &m_id);
	glActiveTexture(slot);
	glBindTexture(tex_type, m_id);

	glTexParameteri(tex_type, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(tex_type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(tex_type, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(tex_type, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(tex_type, 0, GL_RGBA, widthImg, heightImg, 0, format, pixel_type, bytes);
	__glewGenerateMipmap(tex_type);

	stbi_image_free(bytes);
	glBindTexture(tex_type, 0);
}

const GLuint& Texture::GetID() const
{
	return m_id;
}

const GLuint& Texture::GetType() const
{
	return m_type;
}

void Texture::TexUnit(Shader& shader, const std::string& uniform, GLuint unit)
{
	GLuint texUni = glGetUniformLocation(shader.GetID(), uniform.c_str());
	shader.Activate();
	glUniform1i(texUni, unit);
}

void Texture::Bind(GLuint slot)
{
	glActiveTexture(slot);
	glBindTexture(m_type, m_id);
}

void Texture::Unbind() { glBindTexture(m_type, 0); }

void Texture::Delete() { glDeleteTextures(1, &m_id); }