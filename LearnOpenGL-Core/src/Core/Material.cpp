#include "Core/Material.h"

namespace GLCore
{
	Material::Material(const std::string& vertShaderPath, const std::string& fragShaderPath,
					   const std::vector<std::string>& texturePaths)
	{
		m_shader = std::make_unique<GLShader>(vertShaderPath, fragShaderPath);
		if (!texturePaths.empty())
		{
			for (auto& texture : texturePaths)
			{
				m_textures.push_back(std::make_unique<GLTexture>(texture));
			}
		}
	}

	Material::~Material()
	{
		m_shader.reset();
		for (auto& m_texture : m_textures)
		{
			m_texture.reset();
		}
		m_textures.clear();
	}

	void Material::bind() const
	{
		if (m_shader)	m_shader->bind();
		if (!m_textures.empty())
		{
			for (unsigned i = 0; i < m_textures.size(); ++i)
			{
				if (i > 8)
					LOG_WARN(std::format("[Material] texture binded over 8! (Warn for Android)"));
				m_textures.at(i)->bind(i);
			}
		}
	}

	void Material::unbind() const
	{
		if (m_shader)	m_shader->unbind();
		if (!m_textures.empty())
		{
			for (auto& texture : m_textures)
			{
				texture->unbind();
			}
		}
	}

}