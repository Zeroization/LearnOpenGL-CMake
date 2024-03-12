#pragma once

#include "pch.hpp"
#include "OpenGL/glShader.h"
#include "OpenGL/glTexture.h"

class Material
{
public:
	Material(const std::string& vertShaderPath, const std::string& fragShaderPath,
			 const std::vector<std::string>& texturePaths);
	virtual ~Material();

	void bind() const;

	inline bool isShaderEmpty() const { return m_shader == nullptr; }
	inline bool isTexturesEmpty() const { return m_textures.empty(); }

	template<typename T>
	inline void setUniform(const std::string& name, const T& val) const
	{
		m_shader->bind();
		m_shader->setUniform(name, val);
	}

private:
	std::unique_ptr<GLShader> m_shader;
	std::vector<std::unique_ptr<GLTexture>> m_textures;
};
