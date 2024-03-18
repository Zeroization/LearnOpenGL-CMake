#pragma once

#include "pch.hpp"
#include "OpenGL/glShader.h"
#include "OpenGL/glTexture.h"

namespace GLCore
{
	struct BasicMaterial
	{
		glm::vec3 ambient = glm::vec3(0.0f);
		glm::vec3 diffuse = glm::vec3(0.0f);
		glm::vec3 specular = glm::vec3(0.0f);
		float shininess = 0.0f;
	};

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
		inline const GLShader& getShader() const { return *m_shader; }
		inline void setBasicMaterial(const glm::vec3& ambient,
									 const glm::vec3& diffuse,
									 const glm::vec3& specular,
									 float shininess)
		{
			m_basicMaterial = {ambient, diffuse, specular, shininess};
		}
		inline BasicMaterial* getBasicMaterial() { return &m_basicMaterial; }
		template<typename T>
		inline void setUniform(const std::string& name, const T& val) const
		{
			m_shader->bind();
			m_shader->setUniform(name, val);
		}

		// TODO: ���ܻ�� add/remove(a)Texture(s)()
		inline void resetTextures(const std::initializer_list<std::string>& textures)
		{
			// �ȰѾɵĲ��ʶ�������
			for (auto& texture : m_textures)
			{
				texture.reset();
			}
			m_textures.clear();

			// Ȼ����µĲ���
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
		BasicMaterial m_basicMaterial;
	};
}