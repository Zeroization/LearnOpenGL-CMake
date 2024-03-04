#pragma once

#include "pch.hpp"
#include <vector>

// #define OLD_GL_DEBUG
#define NEW_GL_DEBUG

#define GL_ASSERT(x) if (!(x)) __debugbreak()

// [OLD] OpenGL����vvv
#ifdef OLD_GL_DEBUG
namespace GLDebug
{
	void GLClearError();
	bool GLLogCall(const char* functionName, const char* filename, int line);
}

// MSVC�����������ж�
#define GLDebug(x) GLDebug::GLClearError();\
	x;\
	GL_ASSERT(GLDebug::GLLogCall(#x, __FILE__, __LINE__))
#endif
// [OLD] OpenGL����^^^

// [NEW] OpenGL����vvv
#ifdef NEW_GL_DEBUG
#define GLCall(x) GLDebug::GLLogCall(#x, __FILE__, __LINE__);\
	x;\
	glPopDebugGroup()

namespace GLDebug
{
	void GLLogCall(const char* funcName, const char* filename, int line);

	// �ص�����
	void GLAPIENTRY GLDebugCallback(unsigned source,
									unsigned type,
									unsigned id,
									unsigned severity,
									int length,
									const char* message,
									const void* userParam);
	void GLEnableDebugging();
}

#endif
// [NEW] OpenGL����^^^
