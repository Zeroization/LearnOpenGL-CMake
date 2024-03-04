#pragma once

#include <string>
#include <unordered_map>

struct ShaderProgramSource
{
	std::string vertexSource;
	std::string fragmentSource;
};

class Shader
{
public:
	Shader(const std::string& vertexFilepath, const std::string& fragFilepath);
	~Shader();

	void bind() const;
	void unbind() const;

	void setUniform1i(const std::string& name, int value);
	void setUniform1f(const std::string& name, float value);
	void setUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void setUniformMat4f(const std::string& name, const glm::mat4& matrix);

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