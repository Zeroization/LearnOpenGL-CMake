#pragma once

#include "pch.hpp"
#include "Core/Material.h"

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

	inline GLIndexBuffer* getIBO() const { return m_IBO.get(); }
	inline size_t getVBOSize() const { return m_verticesSize; }
	inline GLVertexBufferLayout* getVBOLayout() const { return m_VBLayout.get(); }

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

private:
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

	// 材质
	std::unique_ptr<Material> m_material;
};
