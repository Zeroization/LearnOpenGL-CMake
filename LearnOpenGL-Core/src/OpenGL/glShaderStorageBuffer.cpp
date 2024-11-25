#include <OpenGL/glDebug.h>
#include <OpenGL/glShaderStorageBuffer.h>

namespace GLCore
{
	GLShaderStorageBuffer::GLShaderStorageBuffer(size_t bufferSize)
	{
		m_InitBufferSize = bufferSize;

		GLCall(glGenBuffers(1, &m_rendererID));
		bind();
		GLCall(glBufferData(GL_SHADER_STORAGE_BUFFER, m_InitBufferSize, NULL, GL_DYNAMIC_COPY));
		unbind();
	}

	GLShaderStorageBuffer::~GLShaderStorageBuffer()
	{
		GLCall(glDeleteBuffers(1, &m_rendererID));
	}

	void GLShaderStorageBuffer::bind() const
	{
		GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_rendererID));
	}

	void GLShaderStorageBuffer::unbind() const
	{
		GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0));
	}
}
