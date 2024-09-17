#include "Geometry/Mesh.h"


namespace GLCore
{
	Mesh::Mesh(const std::vector<MeshVertex>& vertices, const std::vector<unsigned>& indices, const std::vector<unsigned int>& textureIndex,
			   std::vector<std::shared_ptr<GLTexture>>* pTextures)
		: m_vertices(vertices), m_indices(indices), m_textureIndex(textureIndex), m_pTextures(pTextures)
	{
		setupMesh();
	}

	void Mesh::onRender(const GLShader& shader, const Renderer& renderer)
	{
		unsigned int diffuseCnt = 1;
		unsigned int specularCnt = 1;
		unsigned int reflectCnt = 1;

		for (unsigned int i = 0; i < m_textureIndex.size(); ++i)
		{
			// TODO: 可能会把 texture_diffuseX 改为 texture_diffuse[x]
			std::string name;
			unsigned int index = m_textureIndex.at(i);
			GLCore::TextureType type = m_pTextures->at(index)->getTextureType();
			switch (type)
			{
				case TextureType::DiffuseMap:
					name = "texture_diffuse";
					// 0 留给SkyBox
					shader.setUniform(std::format("{}{}", name, diffuseCnt++), i + 1);
					break;
				case TextureType::SpecularMap:
					name = "texture_specular";
					shader.setUniform(std::format("{}{}", name, specularCnt++), i + 1);
					break;
				case TextureType::AmbientMap:
					name = "texture_reflect";
					shader.setUniform(std::format("{}{}", name, reflectCnt++), i + 1);
					break;
				case TextureType::Unknown:
				case TextureType::CubeMap:
				case TextureType::EmitMap:
				case TextureType::NormalMap:
				case TextureType::HeightMap:
					LOG_ERROR(std::format("[Mesh]draw(): Try to use an unimplented shader! (emum: {})",
										  static_cast<int>(type)));
					break;
			}
			m_pTextures->at(index)->bind(i + 1);
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
