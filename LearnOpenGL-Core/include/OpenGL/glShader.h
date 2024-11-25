#pragma once

#include "OpenGL/glDebug.h"

#include <string>
#include <unordered_map>

namespace GLCore
{
	struct ShaderProgramSource
	{
		std::string vertexSource;
		std::string fragmentSource;
	};

	class GLShader
	{
	public:
		GLShader(const std::string& vertexFilepath, const std::string& fragFilepath);
		~GLShader();

		void bind() const;
		void unbind() const;

		unsigned int getID() const { return m_rendererID; }

		template<typename T>
		void setUniform(const std::string& name, const T& value) const
		{
			LOG_CRITICAL(std::format("[{}]: setUnifrom(): Type {} is not implemented yet!", "Shader", typeid(T).name()));
			__debugbreak();
		}

		template<>
		void setUniform(const std::string& name, const unsigned int& value) const
		{
			GLCall(glUniform1i(getUniformLocation(name), value));
		}

		template<>
		void setUniform(const std::string& name, const int& value) const
		{
			GLCall(glUniform1i(getUniformLocation(name), value));
		}

		template<>
		void setUniform(const std::string& name, const bool& value) const
		{
			GLCall(glUniform1i(getUniformLocation(name), value ? GL_TRUE : GL_FALSE));
		}

		template<>
		void setUniform(const std::string& name, const float& value) const
		{
			GLCall(glUniform1f(getUniformLocation(name), value));
		}

		template<>
		void setUniform(const std::string& name, const glm::vec2& value) const
		{
			GLCall(glUniform2f(getUniformLocation(name), value.x, value.y));
		}

		template<>
		void setUniform(const std::string& name, const glm::vec3& value) const
		{
			GLCall(glUniform3f(getUniformLocation(name), value.x, value.y, value.z));
		}

		template<>
		void setUniform(const std::string& name, const glm::vec4& value) const
		{
			GLCall(glUniform4f(getUniformLocation(name), value.x, value.y, value.z, value.w));
		}

		template<>
		void setUniform(const std::string& name, const glm::mat2x4& value) const
		{
			GLCall(glUniformMatrix2x4fv(getUniformLocation(name), 1, GL_FALSE, &value[0][0]));
		}

		template<>
		void setUniform(const std::string& name, const glm::mat3& value) const
		{
			GLCall(glUniformMatrix3fv(getUniformLocation(name), 1, GL_FALSE, &value[0][0]));
		}

		template<>
		void setUniform(const std::string& name, const glm::mat4& value) const
		{
			GLCall(glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &value[0][0]));
		}

	private:
		int getUniformLocation(const std::string& name) const;
		ShaderProgramSource parseShader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
		unsigned int compileShader(unsigned int type, const std::string& source);
		unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader);

	private:
		unsigned int m_rendererID;
		std::string m_vertexFilepath, m_fragFilePath;
		mutable std::unordered_map<std::string, int> m_uniformLocationCache;
	};
}