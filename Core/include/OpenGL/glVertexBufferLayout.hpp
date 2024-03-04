#pragma once

#include "glad\glad.h"

#include <iostream>
#include <vector>

// 用于glVertexAttribPointer()的参数
struct GLVertexBufferElement
{
	unsigned int glType;
	unsigned int count;
	unsigned char normalized;

	static unsigned int getSizeOfType(unsigned int glType)
	{
		switch (glType)
		{
			case GL_FLOAT:			return 4;
			case GL_UNSIGNED_INT:	return 4;
			case GL_UNSIGNED_BYTE:	return 1;
		}
		std::cerr << "未知类型: " << glType << std::endl;
		__debugbreak();
		return 0;
	}
};

// 存储VBO中顶点属性布局情况
class GLVertexBufferLayout
{
public:
	GLVertexBufferLayout()
		: m_stride(0) {}

	template<typename T>
	void push(unsigned int count)
	{
		std::cerr << "该类型的Layout还未定义" << std::endl;
		__debugbreak();
	}

	template<>
	void push<float>(unsigned int count)
	{
		m_elements.push_back({GL_FLOAT, count, GL_FALSE});
		m_stride += count * GLVertexBufferElement::getSizeOfType(GL_FLOAT);
	}

	template<>
	void push<unsigned int>(unsigned int count)
	{
		m_elements.push_back({GL_UNSIGNED_INT, count, GL_FALSE});
		m_stride += count * GLVertexBufferElement::getSizeOfType(GL_UNSIGNED_INT);
	}

	template<>
	void push<unsigned char>(unsigned int count)
	{
		m_elements.push_back({GL_UNSIGNED_BYTE, count, GL_TRUE});
		m_stride += count * GLVertexBufferElement::getSizeOfType(GL_UNSIGNED_BYTE);
	}

	inline unsigned int getStride() const { return m_stride; }
	inline const std::vector<GLVertexBufferElement>& getElements() const { return m_elements; }

private:
	std::vector<GLVertexBufferElement> m_elements;
	unsigned int m_stride;
};
