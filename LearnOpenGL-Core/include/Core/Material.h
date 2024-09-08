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

	public:
		void setAmbient(const glm::vec3& color) { ambient = color; }
	};

	class Material
	{
	public:
		Material(const std::string& vertShaderPath, const std::string& fragShaderPath);
		Material(const std::string& vertShaderPath, const std::string& fragShaderPath,
		         const std::vector<TextureDesc>& textureDescList);

		void bind() const;
		void unbind() const;

		// TODO: 可能会加 add/remove(a)Texture(s)()
		void resetTextures(const std::initializer_list<TextureDesc>& textureDescList);
		void resetTextures(const std::vector<std::shared_ptr<GLTexture>>& textures);

		inline void addTexture(GLTexture* texPtr)
		{
			m_textures.push_back(std::make_shared<GLCore::GLTexture>(*texPtr));
		}
		inline bool isShaderEmpty() const { return m_shader == nullptr; }
		inline bool isTexturesEmpty() const { return m_textures.empty(); }
		inline const GLShader& getShader() const { return *m_shader; }
		inline std::unique_ptr<GLShader>& getShaderPtr() { return m_shader; }
		inline void setBasicMaterial(const glm::vec3& ambient,
									 const glm::vec3& diffuse,
									 const glm::vec3& specular,
									 float shininess)
		{
			m_basicMaterial = {ambient, diffuse, specular, shininess};
		}
		inline BasicMaterial* getBasicMaterial() { return &m_basicMaterial; }
		inline const std::vector<std::shared_ptr<GLTexture>>& getTextures() const { return m_textures; }
		template<typename T>
		inline void setUniform(const std::string& name, const T& val) const
		{
			m_shader->bind();
			m_shader->setUniform(name, val);
		}

	private:
		std::unique_ptr<GLShader> m_shader;
		std::vector<std::shared_ptr<GLTexture>> m_textures;
		BasicMaterial m_basicMaterial;
	};
}