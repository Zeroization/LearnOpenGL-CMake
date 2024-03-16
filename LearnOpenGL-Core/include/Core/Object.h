#pragma once

#include "pch.hpp"

#include "Core/Renderer.h"
#include "Core/Material.h"
#include "Core/UUID.h"

#include "OpenGL/glIndexBuffer.h"
#include "OpenGL/glVertexArray.h"
#include "OpenGL/glVertexBuffer.h"
#include "OpenGL/glVertexBufferLayout.hpp"

class GLObject
{
public:
	GLObject(float vertices[], size_t verticesSize, const GLVertexBufferLayout& vertLayout,
			 const std::string& vertPath, const std::string& fragPath,
			 const std::vector<std::string>& texturePaths = {});
	GLObject(float vertices[], size_t verticesSize, const GLVertexBufferLayout& vertLayout,
			 unsigned int* indices, int iCount,
			 const std::string& vertPath, const std::string& fragPath,
			 const std::vector<std::string>& texturePaths = {});
	virtual ~GLObject();

	void bind() const;
	void unbind() const;

	virtual void onRender(const Renderer& renderer);
	virtual void onImGuiRender(const std::string& ObjectName);
	virtual void onUpdate() {}

	inline GLIndexBuffer* getIBO() const { return m_IBO.get(); }
	inline size_t getVBOSize() const { return m_verticesSize; }
	inline GLVertexBufferLayout* getVBOLayout() const { return m_VBLayout.get(); }
	inline bool isVisible() const { return m_isVisible; }
	inline void setVisibility(bool isVisible) { m_isVisible = isVisible; }
	inline const glm::vec3& getScale() const { return m_scale; }
	inline void setScale(float x, float y, float z) { m_scale = glm::vec3(x, y, z); }
	inline const glm::vec3& getRotation() const { return m_rotation; }
	inline void setRotation(float x, float y, float z) { m_rotation = glm::vec3(x, y, z); }
	inline const glm::vec3& getTranslation() const { return m_translation; }
	inline void setTranslation(float x, float y, float z) { m_translation = glm::vec3(x, y, z); }
	inline glm::mat4 getModelMat() const
	{
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), m_scale);
		glm::mat4 rotate = glm::mat4_cast(glm::quat(m_rotation));
		glm::mat4 translate = glm::translate(glm::mat4(1.0f), m_translation);

		return translate * rotate * scale;
	}
	template <typename T>
	void setUniform(const std::string& name, const T& val)
	{
		if (m_material && !m_material->isShaderEmpty())
		{
			m_material->setUniform(name, val);
		}
		else
		{
			LOG_ERROR("[{0}] {1}", "Object", "Material or Shader is nullptr!");
		}
	}
	void resetTextures(const std::initializer_list<std::string>& list) const;

protected:
	// 数据
	float* m_verticesCache = nullptr;
	size_t m_verticesSize = 0;
	unsigned* m_indicesCache = nullptr;
	int m_indicesCount = 0;

	// OpenGL对象
	std::unique_ptr<GLVertexBuffer> m_VBO;
	std::unique_ptr<GLVertexBufferLayout> m_VBLayout;
	std::unique_ptr<GLVertexArray> m_VAO;
	std::unique_ptr<GLIndexBuffer> m_IBO;

	// 属性
	UUID m_uuid;
	bool m_isVisible;

	glm::vec3 m_scale;
	glm::vec3 m_rotation;
	glm::vec3 m_translation;

	std::unique_ptr<Material> m_material;
};
