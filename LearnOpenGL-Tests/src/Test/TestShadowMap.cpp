#include "Test/TestShadowMap.h"

#include "Render/Light/DirectionalLight.h"

namespace test
{
	static std::string proj_res_path(PROJ_RES_PATH);

	TestShadowMap::TestShadowMap()
	{
		m_pShadowShader = std::make_unique<GLCore::GLShader>(
			std::string(proj_res_path + "/Shaders/ShadowMap/shadowmap.vert"),
			std::string(proj_res_path + "/Shaders/ShadowMap/shadowmap.frag")
		);
	}

	void TestShadowMap::onUpdate(float deltaTime, const Input& hardwareInput)
	{
		TestMultipleLights::onUpdate(deltaTime, hardwareInput);

		if (hardwareInput.screenWidth != 0 && hardwareInput.screenHeight != 0)
		{
			m_testWindowWidth = hardwareInput.screenWidth;
			m_testWindowHeight = hardwareInput.screenHeight;
		}

		// 补上光源的FBO
		while (m_shadowMapFBOs.size() < m_pLights.size())
		{
			m_shadowMapFBOs.push_back(std::make_unique<GLCore::GLFrameBuffer>());
			m_shadowMapFBOs.at(m_shadowMapFBOs.size() - 1)->addTextureAttachment(GLCore::FBAttachmentType::DepthAttachment);
		}
		// 移除多余的FBO
		while (m_shadowMapFBOs.size() > m_pLights.size())
		{
			m_shadowMapFBOs.pop_back();
		}
	}

	void TestShadowMap::onRender()
	{
		// 保留原来的摄像机
		std::unique_ptr<GLCore::Camera> originCam;
		originCam.swap(m_pCamera);

		// 先用光源视角渲染
		GLCall(glViewport(0, 0, 1024, 1024));
		for (int i = 0; i < m_pLights.size(); ++i)
		{
			if (m_pLights.at(i)->getLightType() == GLCore::LightType::DirectionalLight)
			{
				// 平行光使用正交投影
				GLCore::DirectionalLight* dirLight = dynamic_cast<GLCore::DirectionalLight*>(m_pLights.at(i).get());
				glm::vec3 dirLightPos = dirLight->getTranslation();
				glm::vec3 dirLightDir = dirLight->getDirection();
				m_proj = m_pCamera->getOrthoProjMat();
				m_view = glm::lookAt(dirLightPos,
									 glm::vec3(dirLightDir + dirLightPos),
									 glm::vec3(0.0f, 1.0f, 0.0f));
			}

			// 设置正确的VP, 以转换到光源视角
			glm::mat4 lightSpaceVP = m_proj * m_view;

			// 开始渲染
			{
				GLCore::Renderer renderer(nullptr);
				for (auto& object : m_pObjects)
				{
					const GLCore::ModelData* data = object->getModelData();
					// 我自闭了, 代码结构太乱了
					if (object->isVisible())
					{
						object->bind();
						m_shadowMapFBOs.at(i)->bindFBO();
						m_pShadowShader->bind();
						m_pShadowShader->setUniform("u_LightSpaceMVP", lightSpaceVP * object->getModelMat());

						GLCall(glClear(GL_DEPTH_BUFFER_BIT));

						if (object->getDataType() == GLCore::ModelDataType::RAW)
						{
							if (data->pRaw->IBO)
							{
								renderer.draw(*data->pRaw->VAO, *data->pRaw->IBO, *m_pShadowShader);
							}
							else
							{
								renderer.draw(*data->pRaw->VAO, object->getVBOSize() / object->getVBOLayout()->getStride(), *m_pShadowShader);
							}
						}
						else if (object->getDataType() == GLCore::ModelDataType::CUSTOM)
						{
							for (auto& mesh : data->pCustom->meshes)
							{
								mesh.onRender(*m_pShadowShader, renderer);
							}
						}
						m_shadowMapFBOs.at(i)->unbindFBO();
						m_pShadowShader->unbind();
					}
				}
			}
		}

		// 别忘了把相机变回来
		m_pCamera.swap(originCam);
		GLCall(glViewport(0, 0, m_testWindowWidth, m_testWindowHeight));
		TestMultipleLights::onRender();
	}

	void TestShadowMap::onImGuiRender()
	{
		ImGui::Begin("Test ShadowMap##TestShadowMap");
		for (int i = 0; i < m_shadowMapFBOs.size(); ++i)
		{
			GLCore::GLTexture* tex = m_shadowMapFBOs.at(i)->getTextures(GLCore::FBAttachmentType::DepthAttachment).at(0);
			// Texture must be bound first
			tex->bind(1);
			GLint swizzle[4] = {
				GL_RED,		// Shader Red   channel source = Texture Red
				GL_RED,		// Shader Green channel source = Texture Green
				GL_RED,		// Shader Blue  channel source = Texture Blue
				GL_ONE		// Shader Alpha channel source = One
			};
			glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzle);
			tex->unbind();
			ImGui::Image((void*)(intptr_t)tex->getID(), ImVec2(128.0f, 128.0f));
		}
		ImGui::End();
		TestMultipleLights::onImGuiRender();
	}
}
