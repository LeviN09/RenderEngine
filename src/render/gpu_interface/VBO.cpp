#include "render/gpu_interface/VBO.hpp"

VBO::VBO(GLfloat* vertices, GLsizeiptr size)
{
	glGenBuffers(1, &m_id);
	glBindBuffer(GL_ARRAY_BUFFER, m_id);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

VBO::~VBO()
{
	Delete();
}

GLuint VBO::GetID() const
{
	return m_id;
}

void VBO::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_id);
}

void VBO::Unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::Delete()
{
	glDeleteBuffers(1, &m_id);
}