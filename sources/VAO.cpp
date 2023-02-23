#include "../headers/VAO.hpp"

// Constructor that generates a VAO ID
VAO::VAO()
{
	glGenVertexArrays(1, &ID);
}

// Links a VBO to the VAO using a certain layout
void VAO::LinkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset)
{
	//std::cout << "link attrib VAO " << ID << std::endl;
	VBO.Bind();
	glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
	glEnableVertexAttribArray(layout);
	VBO.Unbind();
}

// Binds the VAO
void VAO::Bind()
{
	glBindVertexArray(ID);
	//std::cout << "bound VAO " << ID << std::endl;
}

// Unbinds the VAO
void VAO::Unbind()
{
	//std::cout << "unbound VAO " << ID << std::endl;
	glBindVertexArray(0);
}

void VAO::ActivateLayout(GLuint layout) {
	glEnableVertexAttribArray(layout);
}

void VAO::DisableLayout(GLuint layout) {
	glDisableVertexAttribArray(layout);
}

// Deletes the VAO
void VAO::Delete()
{
	glDeleteVertexArrays(1, &ID);
}