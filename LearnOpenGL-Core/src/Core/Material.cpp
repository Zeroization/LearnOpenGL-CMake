#include "Core/Material.h"

namespace GLCore
{
	Material::Material(const std::string& vertShaderPath, const std::string& fragShaderPath,
					   const std::vector<TextureData>& textureDataList)
		: Material(vertShaderPath, fragShaderPath)
	{
		if (!textureDataList.empty())
		{
			for (auto& textureData : textureDataList)
			{
				m_textures.push_back(std::make_shared<GLTexture>(textureData));
			}
		}
	}

	Material::Material(const std::string& vertShaderPath, const std::string& fragShaderPath)
	{
		m_shader = std::make_unique<GLShader>(vertShaderPath, fragShaderPath);
	}

	void Material::bind() const
	{
		if (m_shader)	m_shader->bind();
		if (!m_textures.empty())
		{
			for (unsigned i = 0; i < m_textures.size(); ++i)
			{
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

	void Material::resetTextures(const std::initializer_list<TextureDesc>& textureDescList)
	{
		// 先把旧的材质都清理了
		m_textures.clear();

		// 然后绑定新的材质
		if (textureDescList.size())
		{
			for (auto& textureDesc : textureDescList)
			{
				m_textures.push_back(std::make_unique<GLTexture>(textureDesc));
			}
		}
		else
		{
			LOG_WARN("[Material] Textures are reseted and have no elements!!");
		}
	}

	void Material::resetTextures(const std::vector<std::shared_ptr<GLTexture>>& textures)
	{
		// 先把旧的材质都清理了
		m_textures.clear();

		// 然后绑定新的材质
		if (!textures.empty())
		{
			for (auto& texture : textures)
			{
				m_textures.push_back(texture);
			}
		}
		else
		{
			LOG_WARN("[Material] Textures are reseted and have no elements!!");
		}
	}
}
