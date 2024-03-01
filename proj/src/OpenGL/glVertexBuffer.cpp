#include "OpenGL/glVertexBuffer.h"
#include "OpenGL/glDebug.h"

GLVertexBuffer::GLVertexBuffer(const void* data, unsigned size)
{
	GLCall(glGenBuffers(1, &m_rendererID));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_rendererID));
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

GLVertexBuffer::~GLVertexBuffer()
{
	GLCall(glDeleteBuffers(1, &m_rendererID));
}

void GLVertexBuffer::bind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_rendererID));
}

void GLVertexBuffer::unbind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
