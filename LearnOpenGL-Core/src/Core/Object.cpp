#include "Core/Object.h"

namespace GLCore
{
	GLObject::GLObject(float vertices[], size_t verticesSize, const GLVertexBufferLayout& vertLayout,
					   const std::string& vertPath, const std::string& fragPath,
					   const std::vector<TextureData>& textureDataList)
		: m_verticesCache(vertices), m_verticesSize(verticesSize),
		m_isVisible(true)
	{
		m_VBO = std::make_unique<GLVertexBuffer>(vertices, verticesSize);
		m_VBLayout = std::make_unique<GLVertexBufferLayout>(vertLayout);
		m_VAO = std::make_unique<GLVertexArray>();
		m_VAO->addVBO(*m_VBO, *m_VBLayout);

		m_material = std::make_unique<Material>(vertPath, fragPath, textureDataList);
		m_color = glm::vec3(1.0f, 0.0f, 1.0f);
		m_basicMaterial = m_material->getBasicMaterial();
		m_basicMaterial->ambient = glm::vec3(0.09f, 0.09f, 0.09f);
		m_basicMaterial->diffuse = m_color;
		m_basicMaterial->specular = glm::vec3(0.5f, 0.5f, 0.5f);
		m_basicMaterial->shininess = 0.1f;
		if (textureDataList.empty())
		{
			LOG_WARN("[GLObject] load no texture !!!");
		}

		m_scale = glm::vec3(1.0f);
		m_translation = glm::vec3(0.0f);
		m_rotation = glm::vec3(0.0f);
	}

	GLObject::GLObject(float vertices[], size_t verticesSize, const GLVertexBufferLayout& vertLayout,
					   unsigned int* indices, int iCount,
					   const std::string& vertPath, const std::string& fragPath,
					   const std::vector<TextureData>& textureDataList)
		: GLObject(vertices, verticesSize, vertLayout, vertPath, fragPath, textureDataList)
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
		if (m_IBO)
			m_IBO->bind();
	}

	void GLObject::unbind() const
	{
		m_material->unbind();
		m_VAO->unbind();
		if (m_IBO)
			m_IBO->unbind();
	}

	void GLObject::onRender(const Renderer& renderer)
	{
		// 这里可能要改
		if (m_isVisible)
		{
			this->bind();
			if (m_IBO)
			{
				renderer.draw(*m_VAO, *m_IBO, m_material->getShader());
			}
			else
			{
				// LOG_DEBUG(std::format("[GLObject] draw without ibo, pointsCount = {}.", this->getVBOSize() / this->getVBOLayout()->getStride()));
				renderer.draw(*m_VAO, this->getVBOSize() / this->getVBOLayout()->getStride(), m_material->getShader());
			}
		}
	}

	void GLObject::onImGuiRender(const std::string& ObjectName)
	{
		const std::string objID("##" + std::to_string(m_uuid()));
		if (ImGui::CollapsingHeader((ObjectName + objID).c_str()))
		{
			ImGui::SeparatorText(std::string("Transforms" + objID).c_str());
			ImGui::DragFloat3(std::string("Scale" + objID).c_str(), &m_scale.x, 0.005f, 0.0f, 10.0f);
			ImGui::DragFloat3(std::string("Rotation (Euler Angle)" + objID).c_str(), &m_rotation.x, 0.25f, -360.0f, 360.0f);
			ImGui::DragFloat3(std::string("Translation" + objID).c_str(), &m_translation.x, 0.25f, -100.0f, 100.0f);

			ImGui::SeparatorText(std::string("Attributes" + objID).c_str());
			ImGui::Checkbox(std::string("isVisible" + objID).c_str(), &m_isVisible);

			ImGui::SeparatorText(std::string("Material" + objID).c_str());
			if (m_material->isTexturesEmpty())
			{
				ImGui::DragFloat3(std::string("Ambient" + objID).c_str(), &m_basicMaterial->ambient.r, 0.005f, 0.0f, 1.0f);
				ImGui::DragFloat3(std::string("Diffuse" + objID).c_str(), &m_basicMaterial->diffuse.r, 0.005f, 0.0f, 1.0f);
				ImGui::DragFloat3(std::string("Specular" + objID).c_str(), &m_basicMaterial->specular.r, 0.005f, 0.0f, 1.0f);
				ImGui::DragFloat(std::string("Shininess" + objID).c_str(), &m_basicMaterial->shininess, 0.005f, 0.0f, 1.0f);

				// TODO: 选纹理文件
			}
		}
	}

	void GLObject::resetTextures(const std::initializer_list<TextureData>& list) const
	{
		if (m_material)
		{
			m_material->resetTextures(list);
		}
		else
		{
			LOG_WARN("[Material] resetTextures() when m_material == nullptr !!!");
		}
	}
}