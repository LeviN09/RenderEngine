#include "render/gpu_interface/VAO.hpp"

VAO::VAO()
{
	glGenVertexArrays(1, &m_id);
}

VAO::~VAO()
{
	Delete();
}

GLuint VAO::GetID() const
{
	return m_id;
}

void VAO::LinkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset)
{
	VBO.Bind();
	glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
	glEnableVertexAttribArray(layout);
	VBO.Unbind();
}

void VAO::Bind()
{
	glBindVertexArray(m_id);
}

void VAO::Unbind()
{
	glBindVertexArray(0);
}

void VAO::ActivateLayout(GLuint layout)
{
	glEnableVertexAttribArray(layout);
}

void VAO::DisableLayout(GLuint layout)
{
	glDisableVertexAttribArray(layout);
}

void VAO::Delete()
{
	glDeleteVertexArrays(1, &m_id);
}