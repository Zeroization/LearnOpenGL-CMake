#include "Render/Feature/Skybox.h"

namespace GLCore
{
	// 适用于天空盒的顶点数据
	static float skyBoxVertices[] = {
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f
	};
	static unsigned int skyBoxIndices[] = {
		0, 3, 1, 0, 2, 3,
		1, 5, 4, 1, 4, 0,
		4, 2, 0, 4, 6, 2,
		5, 6, 4, 5, 7, 6,
		2, 6, 7, 2, 7, 3,
		1, 7, 5, 1, 3, 7
	};

	SkyBox::SkyBox(const std::vector<GLCore::CubeMapTexDesc>& cubeTextures)
		: GLObject(skyBoxVertices, sizeof(skyBoxVertices), GLCore::GLVertexBufferLayout({3}),
				   skyBoxIndices, 36, 
				   std::string(PROJ_RES_PATH) + "/Shaders/Skybox/skybox.vert",
				   std::string(PROJ_RES_PATH) + "/Shaders/Skybox/skybox.frag")
	{
		m_cubeMapTexture = std::make_shared<GLCore::GLCubeMapTexture>(cubeTextures);
		resetTextures(std::vector<std::shared_ptr<GLTexture>>({m_cubeMapTexture}));
	}

	// void SkyBox::bind() const
	// {
	// 	m_modelData.pRaw->VAO->bind();
	// 	m_modelData.pRaw->IBO->bind();
	// 	m_cubeMapTexture->bind(0);
	// }
	//
	// void SkyBox::unbind() const
	// {
	// 	GLObject::unbind();
	// }


	// void SkyBox::onRender(const Renderer& renderer)
	// {
	// 	this->bind();
	// 	if (m_modelData.pRaw->IBO)
	// 	{
	// 		renderer.draw(*m_modelData.pRaw->VAO, *m_modelData.pRaw->IBO, m_material->getShader());
	// 	}
	// }

	void SkyBox::onImGuiRender(const std::string& ObjectName)
	{
		// 无需渲染
	}

	void SkyBox::onUpdate()
	{
		setUniform("u_Skybox", 0);
	}
}
