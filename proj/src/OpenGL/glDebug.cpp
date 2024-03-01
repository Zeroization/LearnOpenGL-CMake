#include "OpenGL/glDebug.h"

#pragma once

#include "pch.hpp"

// #define OLD_GL_DEBUG
#define NEW_GL_DEBUG

// [OLD] OpenGL调试vvv
#ifdef OLD_GL_DEBUG
void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* functionName, const char* filename, int line)
{
	while (GLenum error = glGetError())
	{
		std::cerr << std::format("[OpenGL ERROR]:code {:#x} at {}, in {}({}).\n", error, functionName, filename, line);
		return false;
	}
	return true;
}

// MSVC编译器触发中断
#define GL_ASSERT(x) if (!(x)) __debugbreak()
#define GLDebug(x) GLClearError();\
	x;\
	GL_ASSERT(GLLogCall(#x, __FILE__, __LINE__))
#endif
// [OLD] OpenGL调试^^^

// [NEW] OpenGL调试vvv
#ifdef NEW_GL_DEBUG
void GLLogCall(const char* funcName, const char* filename, int line)
{
	std::string simpleFilename(filename);
	simpleFilename = simpleFilename.substr(simpleFilename.find_last_of('\\') + 1);
	const std::string log = std::format("{}({}): {}", simpleFilename, line, funcName);
	glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 1, log.size(), log.c_str());
}

// 回调函数
void GLAPIENTRY GLDebugCallback(unsigned source,
								unsigned type,
								unsigned id,
								unsigned severity,
								int length,
								const char* message,
								const void* userParam)
{
	std::string sourceStr;
	switch (source)
	{
		case GL_DEBUG_SOURCE_API:				sourceStr = "API"; break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:		sourceStr = "Window System"; break;
		case GL_DEBUG_SOURCE_SHADER_COMPILER:	sourceStr = "Shader Compiler"; break;
		case GL_DEBUG_SOURCE_THIRD_PARTY:		sourceStr = "Third Party"; break;
		case GL_DEBUG_SOURCE_APPLICATION:		sourceStr = "Application"; break;
		case GL_DEBUG_SOURCE_OTHER:				sourceStr = "Other"; break;
		default:								sourceStr = "Unknown"; break;
	}

	std::string typeStr;
	switch (type)
	{
		case GL_DEBUG_TYPE_ERROR:				typeStr = "Error"; break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:	typeStr = "Deprecated Behavior"; break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:	typeStr = "Undefined Behavior"; break;
		case GL_DEBUG_TYPE_PORTABILITY:			typeStr = "Portability"; break;
		case GL_DEBUG_TYPE_PERFORMANCE:			typeStr = "Performance"; break;
		case GL_DEBUG_TYPE_MARKER:				typeStr = "Marker"; break;
		case GL_DEBUG_TYPE_PUSH_GROUP:			typeStr = "Push Group"; break;
		case GL_DEBUG_TYPE_POP_GROUP:			typeStr = "Pop Group"; break;
		case GL_DEBUG_TYPE_OTHER:				typeStr = "Other"; break;
		default:								typeStr = "Unknown"; break;
	}

	// ANSI转义码实现改变控制台颜色
	std::string severityStr;
	switch (severity)
	{
		case GL_DEBUG_SEVERITY_HIGH:			severityStr = "\033[38;5;9m"; break;	// 红色
		case GL_DEBUG_SEVERITY_MEDIUM:			severityStr = "\033[38;5;202m"; break; // 橙色
		case GL_DEBUG_SEVERITY_LOW:				severityStr = "\033[38;5;220m"; break;	// 黄色
		case GL_DEBUG_SEVERITY_NOTIFICATION:
		default:								severityStr = "\033[38;5;231m"; break; // 白色
	}

	std::cerr << std::format("[OpenGL {}] [{}{}({})\033[0m] {}",
							 sourceStr, severityStr, typeStr, id, message) << '\n';
}
#endif
// [NEW] OpenGL调试^^^
