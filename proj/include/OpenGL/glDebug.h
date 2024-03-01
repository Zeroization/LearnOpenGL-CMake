#pragma once

#include "pch.hpp"

// #define OLD_GL_DEBUG
#define NEW_GL_DEBUG

#define GL_ASSERT(x) if (!(x)) __debugbreak()

// [OLD] OpenGL调试vvv
#ifdef OLD_GL_DEBUG
void GLClearError();

bool GLLogCall(const char* functionName, const char* filename, int line);

// MSVC编译器触发中断
#define GLDebug(x) GLClearError();\
	x;\
	GL_ASSERT(GLLogCall(#x, __FILE__, __LINE__))
#endif
// [OLD] OpenGL调试^^^

// [NEW] OpenGL调试vvv
#ifdef NEW_GL_DEBUG
#define GLCall(x) GLLogCall(#x, __FILE__, __LINE__);\
	x;\
	glPopDebugGroup()

void GLLogCall(const char* funcName, const char* filename, int line);

// 回调函数
void GLAPIENTRY GLDebugCallback(unsigned source,
								unsigned type,
								unsigned id,
								unsigned severity,
								int length,
								const char* message,
								const void* userParam);
#endif
// [NEW] OpenGL调试^^^
