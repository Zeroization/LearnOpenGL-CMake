#include "OpenGL/glDebug.h"

#pragma once

#include "pch.hpp"

// #define OLD_GL_DEBUG
#define NEW_GL_DEBUG

// [OLD] OpenGL调试vvv
#ifdef OLD_GL_DEBUG
namespace GLCore
{
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
}
#endif
// [OLD] OpenGL调试^^^

// [NEW] OpenGL调试vvv
#ifdef NEW_GL_DEBUG
namespace GLCore
{
	struct debugMsg
	{
		std::string msg;
		unsigned int id;
	};

	void GLLogCall(const char* funcName, const char* filename, int line)
	{
		std::string simpleFilename(filename);
		simpleFilename = simpleFilename.substr(simpleFilename.find_last_of('\\') + 1);
		const std::string log = std::format("{}({}): {}", simpleFilename, line, funcName);
		// todo: 以后上下文多了可能会改ID
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

		static std::vector<debugMsg> msgQueue;

		// LOG_TRACE("{0}", std::format("[GL {}] [{}({})] {}", sourceStr, typeStr, id, message));

		if (type == GL_DEBUG_TYPE_POP_GROUP)
		{
			if (!msgQueue.empty() && msgQueue.at(msgQueue.size() - 1).id != 1)
			{
				for (auto& debugMsg : msgQueue)
				{
					switch (severity)
					{
						case GL_DEBUG_SEVERITY_HIGH:			LOG_CRITICAL("{0}", debugMsg.msg); GL_ASSERT(false); break;
						case GL_DEBUG_SEVERITY_MEDIUM:			LOG_ERROR("{0}", debugMsg.msg); break;
						case GL_DEBUG_SEVERITY_LOW:				LOG_WARN("{0}", debugMsg.msg); break;
						case GL_DEBUG_SEVERITY_NOTIFICATION:
						default:								LOG_INFO("{0}", debugMsg.msg); break;
					}
				}
				msgQueue.clear();
			}
			else
			{
				msgQueue.clear();
			}
		}
		else
		{
			msgQueue.push_back({
				std::format("[GL {}] [{}({})] {}", sourceStr, typeStr, id, message),
				id
			});
		}
	}

	void GLEnableDebugging()
	{
		// 启用自定义debug
		int flags;
		GLCall(glGetIntegerv(GL_CONTEXT_FLAGS, &flags));
		if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
		{
			glEnable(GL_DEBUG_OUTPUT);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
			glDebugMessageCallback(GLDebugCallback, nullptr);
			glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
		}
	}

}
#endif
// [NEW] OpenGL调试^^^
