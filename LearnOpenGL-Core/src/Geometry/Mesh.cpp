#include "Geometry/Mesh.h"


namespace GLCore
{
	Mesh::Mesh(const std::vector<MeshVertex>& vertices, const std::vector<unsigned>& indices, const std::vector<unsigned int>& textureIndex,
			   std::vector<std::shared_ptr<GLTexture>>* pTextures)
		: m_vertices(vertices), m_indices(indices), m_textureIndex(textureIndex), m_pTextures(pTextures)
	{
		setupMesh();
	}

	void Mesh::onRender(const GLShader& shader, const Renderer& renderer) const
	{
		unsigned int diffuseCnt = 1;
		unsigned int specularCnt = 1;

		for (unsigned int i = 0; i < m_textureIndex.size(); ++i)
		{
			// TODO: 可能会把 texture_diffuseX 改为 texture_diffuse[x]
			std::string name;
			unsigned int index = m_textureIndex.at(i);
			switch (m_pTextures->at(index)->getTextureType())
			{
				case GLCore::TextureType::DiffuseMap:
					name = "texture_diffuse";
					shader.setUniform(std::format("{}{}", name, diffuseCnt++), i);
					break;
				case GLCore::TextureType::SpecularMap:
					name = "texture_specular";
					shader.setUniform(std::format("{}{}", name, specularCnt++), i);
					break;
				case GLCore::TextureType::Unknown:
				case GLCore::TextureType::AmbientMap:
					LOG_ERROR("[Mesh]draw(): Try to use an unimplented shader!");
					break;
			}
			m_pTextures->at(index)->bind(i);
		}

		renderer.draw(*m_pVAO, *m_pIBO, shader);

		m_pVAO->unbind();
		if (!m_textureIndex.empty() && !m_pTextures->empty())
			m_pTextures->at(0)->unbind();
	}

	void Mesh::setupMesh()
	{
		m_pVAO = std::make_unique<GLVertexArray>();
		m_pVBO = std::make_unique<GLVertexBuffer>(m_vertices.data(), m_vertices.size() * sizeof(MeshVertex));
		m_pVAO->addVBO(*m_pVBO, GLVertexBufferLayout({3, 3, 2}));
		m_pIBO = std::make_unique<GLIndexBuffer>(m_indices.data(), m_indices.size());
	}
}
