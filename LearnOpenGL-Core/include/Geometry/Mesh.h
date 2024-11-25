#pragma once

#include "pch.hpp"

#include "Core/Renderer.h"

#include "OpenGL/glTexture.h"
#include "OpenGL/glShader.h"
#include "OpenGL/glVertexArray.h"
#include "OpenGL/glVertexBuffer.h"
#include "OpenGL/glIndexBuffer.h"

#include <map>
#include <array>

namespace GLCore
{
#ifndef MAX_BONE_INFLUENCE
	#define MAX_BONE_INFLUENCE 4
#endif

	struct MeshVertex
	{
		glm::vec3 Position = glm::vec3(0.0f);
		glm::vec3 Normal = glm::vec3(0.0f);
		glm::vec2 TexCoords = glm::vec2(0.0f);

		// 影响该顶点的关节们
		std::array<int, MAX_BONE_INFLUENCE> m_BoneIDs;
		// 关节对应的权重
		std::array<float, MAX_BONE_INFLUENCE> m_BoneWeights;
	};

	class Mesh
	{
	public:
		Mesh(const std::vector<MeshVertex>& vertices, const std::vector<unsigned>& indices, const std::vector<unsigned int>& textureIndex,
			 std::vector<std::shared_ptr<GLTexture>>* pTextures);
		
		void onRender(const GLShader& shader, const Renderer& renderer);

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
