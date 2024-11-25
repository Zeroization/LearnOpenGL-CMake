#include "OpenGL/glVertexArray.h"
#include "OpenGL/glDebug.h"

namespace GLCore
{
	GLVertexArray::GLVertexArray()
	{
		GLCall(glGenVertexArrays(1, &m_rendererID));
	}

	GLVertexArray::~GLVertexArray()
	{
		GLCall(glDeleteVertexArrays(1, &m_rendererID));
	}

	void GLVertexArray::bind() const
	{
		GLCall(glBindVertexArray(m_rendererID));
	}

	void GLVertexArray::unbind() const
	{
		GLCall(glBindVertexArray(0));
	}

	void GLVertexArray::addVBO(const GLVertexBuffer& vbo, const GLVertexBufferLayout& layout) const
	{
		bind();
		vbo.bind();

		const auto& elements = layout.getElements();
		unsigned int offset = 0;
		for (unsigned int i = 0; i < elements.size(); ++i)
		{
			const auto& element = elements.at(i);
			GLCall(glEnableVertexAttribArray(i));

			if (element.glType == GL_INT)
			{
				GLCall(glVertexAttribIPointer(i, element.count, element.glType, layout.getStride(),
											  (const void*)offset));
			}
			else
			{
				GLCall(glVertexAttribPointer(i, element.count, element.glType,
											 element.normalized, layout.getStride(), (const void*)offset));
			}

			offset += element.count * GLVertexBufferElement::getSizeOfType(element.glType);
		}
	}
}