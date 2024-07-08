#include "render/gpu_interface/VBO.hpp"

// Constructor that generates a Vertex Buffer Object and links it to vertices
VBO::VBO(GLfloat* vertices, GLsizeiptr size)
{
	glGenBuffers(1, &m_id);
	//std::cout << "bound VBO " << ID << std::endl;
	glBindBuffer(GL_ARRAY_BUFFER, m_id);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

GLuint VBO::GetID() const
{
	return m_id;
}

// Binds the VBO
void VBO::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_id);
	//std::cout << "bound VBO " << ID << std::endl;
}

// Unbinds the VBO
void VBO::Unbind()
{
	//std::cout << "unbound VBO " << ID << std::endl;
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// Deletes the VBO
void VBO::Delete()
{
	glDeleteBuffers(1, &m_id);
}