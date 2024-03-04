#include "Shader.h"
#include "OpenGL/glDebug.h"

#include <iostream>
#include <fstream>
#include <sstream>

Shader::Shader(const std::string& filepath)
	: m_rendererID(0), m_filePath(filepath)
{
	ShaderProgramSource source = parseShader(filepath);
	m_rendererID = createShader(source.vertexSource, source.fragmentSource);
}

Shader::~Shader()
{
	GLCall(glDeleteProgram(m_rendererID));
}

void Shader::bind() const
{
	GLCall(glUseProgram(m_rendererID));
}

void Shader::unbind() const
{
	GLCall(glUseProgram(0));
}

void Shader::setUniform1i(const std::string& name, int value)
{
	GLCall(glUniform1i(getUniformLocation(name), value));
}

void Shader::setUniform1f(const std::string& name, float value)
{
	GLCall(glUniform1f(getUniformLocation(name), value));
}

void Shader::setUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	GLCall(glUniform4f(getUniformLocation(name), v0, v1, v2, v3));
}

void Shader::setUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
	GLCall(glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

int Shader::getUniformLocation(const std::string& name) const
{
	if (m_uniformLocationCache.contains(name))
	{
		return m_uniformLocationCache[name];
	}

	int location;
	GLCall(location = glGetUniformLocation(m_rendererID, name.c_str()));
	if (location == -1)
	{
		// TODO: spdlog
		std::cerr << "[Shader]: unifrom '" << name << "' doesn't exist!" << std::endl;
	}
	m_uniformLocationCache[name] = location;

	return location;
}

ShaderProgramSource Shader::parseShader(const std::string& filepath)
{
	std::ifstream stream(filepath);
	enum class ShaderType
	{
		NONE = -1, VERTEX, FRAGMENT
	};

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;
	while (std::getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
			{
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos)
			{
				type = ShaderType::FRAGMENT;
			}
		}
		else
		{
			ss[static_cast<int>(type)] << line << "\n";
		}
	}

	return {ss[0].str(), ss[1].str()};
}

unsigned Shader::compileShader(unsigned int type, const std::string& source)
{
	unsigned int id;
	GLCall(id = glCreateShader(type));
	const char* src = source.c_str();
	GLCall(glShaderSource(id, 1, &src, nullptr));
	GLCall(glCompileShader(id));

	int result;
	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
	if (result == GL_FALSE)
	{
		int length;
		GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
		char* errorMsg = static_cast<char*>(_malloca(length * sizeof(char)));
		GLCall(glGetShaderInfoLog(id, length, &length, errorMsg));
		std::cerr << "[Shader]: Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "Shader!\n";
		std::cerr << errorMsg << std::endl;

		GLCall(glDeleteShader(id));
		return 0;
	}

	return id;
}

unsigned Shader::createShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	unsigned int program;
	GLCall(program = glCreateProgram());
	unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

	GLCall(glAttachShader(program, vs));
	GLCall(glAttachShader(program, fs));
	GLCall(glLinkProgram(program));
	GLCall(glValidateProgram(program));

	GLCall(glDeleteShader(vs));
	GLCall(glDeleteShader(fs));

	return program;
}
