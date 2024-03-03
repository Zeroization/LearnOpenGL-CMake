#pragma once

#include "OpenGL\glVertexBuffer.h"
#include "OpenGL\glVertexBufferLayout.hpp"

class GLVertexArray
{
private:
	unsigned int m_rendererID;

public:
	GLVertexArray();
	~GLVertexArray();

	void addVBO(const GLVertexBuffer& vbo, const GLVertexBufferLayout& layout) const;

	void bind() const;
	void unbind() const;
};