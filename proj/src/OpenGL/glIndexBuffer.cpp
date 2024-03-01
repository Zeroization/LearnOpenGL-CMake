#include "OpenGL/glIndexBuffer.h"
#include "OpenGL/glDebug.h"

GLIndexBuffer::GLIndexBuffer(const unsigned int* data, unsigned int count)
	: m_count(count)
{
	GL_ASSERT(sizeof(unsigned int) == sizeof(GLuint));

	GLCall(glGenBuffers(1, &m_rendererID));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

GLIndexBuffer::~GLIndexBuffer()
{
	GLCall(glDeleteBuffers(1, &m_rendererID));
}

void GLIndexBuffer::bind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID));
}

void GLIndexBuffer::unbind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}