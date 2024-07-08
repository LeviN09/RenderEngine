#include "render/gpu_interface/EBO.hpp"

// Constructor that generates a Elements Buffer Object and links it to indices
EBO::EBO(GLuint* indices, GLsizeiptr size)
{
	glGenBuffers(1, &m_id);
	//std::cout << "bound EBO " << ID << std::endl;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
}

GLuint EBO::GetID() const
{
	return m_id;
}

// Binds the EBO
void EBO::Bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
	//std::cout << "bound EBO " << ID << std::endl;
}

// Unbinds the EBO
void EBO::Unbind()
{
	//std::cout << "unbound EBO " << ID << std::endl;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

// Deletes the EBO
void EBO::Delete()
{
	glDeleteBuffers(1, &m_id);
}