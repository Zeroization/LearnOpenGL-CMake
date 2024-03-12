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
	void unbind() const;

	inline bool isShaderEmpty() const { return m_shader == nullptr; }
	inline bool isTexturesEmpty() const { return m_textures.empty(); }

	template<typename T>
	inline void setUniform(const std::string& name, const T& val) const
	{
		m_shader->bind();
		m_shader->setUniform(name, val);
	}
	// TODO: 可能会加 add/remove(a)Texture(s)()
	inline void resetTextures(const std::initializer_list<std::string>& textures)
	{
		// 先把旧的材质都清理了
		for (auto& texture : m_textures)
		{
			texture.reset();
		}
		m_textures.clear();

		// 然后绑定新的材质
		if (textures.size())
		{
			for (auto& texture : textures)
			{
				m_textures.push_back(std::make_unique<GLTexture>(texture));
			}
		}
		else
		{
			LOG_WARN("[Material] Textures are reseted and have no elements!!");
		}
	}

private:
	std::unique_ptr<GLShader> m_shader;
	std::vector<std::unique_ptr<GLTexture>> m_textures;
};
