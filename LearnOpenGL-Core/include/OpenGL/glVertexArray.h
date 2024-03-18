#pragma once

#include "OpenGL\glVertexBuffer.h"
#include "OpenGL\glVertexBufferLayout.hpp"

namespace GLCore
{
	class GLVertexArray
	{
	public:
		GLVertexArray();
		~GLVertexArray();

		void addVBO(const GLVertexBuffer& vbo, const GLVertexBufferLayout& layout) const;

		void bind() const;
		void unbind() const;

	private:
		unsigned int m_rendererID;
	};
}