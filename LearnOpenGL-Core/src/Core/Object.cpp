#include "Core/Object.h"

GLObject::GLObject(float vertices[], size_t verticesSize, const GLVertexBufferLayout& vertLayout,
				   const std::string& vertPath, const std::string& fragPath,
				   const std::vector<std::string>& texturePaths)
	: m_verticesCache(vertices), m_verticesSize(verticesSize)
{
	m_VBO = std::make_unique<GLVertexBuffer>(vertices, verticesSize);
	m_VBLayout = std::make_unique<GLVertexBufferLayout>(vertLayout);
	m_VAO = std::make_unique<GLVertexArray>();
	m_VAO->addVBO(*m_VBO, *m_VBLayout);
	
	m_material = std::make_unique<Material>(vertPath, fragPath, texturePaths);
}

GLObject::GLObject(float vertices[], size_t verticesSize, const GLVertexBufferLayout& vertLayout,
				   unsigned int* indices, int iCount,
				   const std::string& vertPath, const std::string& fragPath,
				   const std::vector<std::string>& texturePaths)
	: GLObject(vertices, verticesSize, vertLayout, vertPath, fragPath, texturePaths)
{
	m_indicesCache = indices;
	m_indicesCount = iCount;
	m_IBO = std::make_unique<GLIndexBuffer>(indices, iCount);
}

GLObject::~GLObject() = default;

void GLObject::bind() const
{
	m_material->bind();
	m_VAO->bind();
	m_IBO->bind();
}

void GLObject::unbind() const
{
}
