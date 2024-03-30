#pragma once

#include "pch.hpp"

#include "Core/Renderer.h"

#include "OpenGL/glTexture.h"
#include "OpenGL/glShader.h"
#include "OpenGL/glVertexArray.h"
#include "OpenGL/glVertexBuffer.h"
#include "OpenGL/glIndexBuffer.h"

namespace GLCore
{
	struct MeshVertex
	{
		glm::vec3 Position = glm::vec3(0.0f);
		glm::vec3 Normal = glm::vec3(0.0f);
		glm::vec2 TexCoords = glm::vec2(0.0f);
	};

	class Mesh
	{
	public:
		Mesh(const std::vector<MeshVertex>& vertices, const std::vector<unsigned>& indices, const std::vector<unsigned int>& textureIndex,
			 std::vector<std::shared_ptr<GLTexture>>* pTextures);
		
		void onRender(const GLShader& shader, const Renderer& renderer) const;

	private:
		void setupMesh();

	private:
		std::vector<MeshVertex> m_vertices;
		std::vector<unsigned int> m_indices;
		std::unique_ptr<GLVertexArray> m_pVAO;
		std::unique_ptr<GLVertexBuffer> m_pVBO;
		std::unique_ptr<GLIndexBuffer> m_pIBO;

		std::vector<unsigned int> m_textureIndex;
		std::vector<std::shared_ptr<GLTexture>>* m_pTextures;
	};
}
