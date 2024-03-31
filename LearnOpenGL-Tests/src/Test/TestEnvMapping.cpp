#include "Test/TestEnvMapping.h"

namespace test
{
	static float vertices[] = {
		// positions          // normals           
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	};
	static std::string proj_res_path(PROJ_RES_PATH);

	TestEnvMapping::TestEnvMapping()
		: TestMultipleLights(), m_envMappingParam(0), m_refractionRatio(1.52f)
	{
		// 创建一个空的CubeMap
		m_pDynamicEnvMap = std::make_unique<GLCore::GLCubeMapTexture>();
		// 初始化FBO, 绑定深度RBO和一个面的纹理, 让它"完整"
		m_pDynEnvMapFBO = std::make_unique<GLCore::GLFrameBuffer>();
		m_pDynEnvMapFBO->addRBOAttachment(GLCore::FBAttachmentType::DepthAttachment);
		m_pDynEnvMapFBO->updateCubeMapTexAttachment(m_pDynamicEnvMap->getID(), GL_TEXTURE_CUBE_MAP_POSITIVE_X);
	}

	void TestEnvMapping::onUpdate(float deltaTime, const Input& hardwareInput)
	{
		if (hardwareInput.screenWidth != 0 && hardwareInput.screenHeight != 0)
		{
			m_testWindowWidth = hardwareInput.screenWidth;
			m_testWindowHeight = hardwareInput.screenHeight;
		}

		// 处理EnvMap物体
		m_view = m_pCamera->getViewMat();
		m_proj = m_pCamera->getPerspectiveProjMat(hardwareInput.screenWidth, hardwareInput.screenHeight);
		for (const auto& envMapObj : m_pEnvMapObjects)
		{
			glm::mat4 model = envMapObj->getModelMat();
			glm::mat3 normalMat = glm::transpose(glm::inverse(glm::mat3(model)));

			envMapObj->setUniform("u_MVP", m_proj * m_view * model);
			envMapObj->setUniform("u_Model", model);
			envMapObj->setUniform("u_Normal", normalMat);
			envMapObj->setUniform("u_CameraPos", m_pCamera->getCameraPos());
			envMapObj->setUniform("u_Skybox", 0);

			if (envMapObj->getDataType() == GLCore::ModelDataType::RAW)
			{
				switch (m_envMappingParam)
				{
					case 1:
						envMapObj->setUniform("u_EnableReflection", true);
						envMapObj->setUniform("u_EnableRefraction", false);
						envMapObj->setUniform("u_EnableDynamicEnvMap", false);
						break;
					case 2:
						envMapObj->setUniform("u_EnableReflection", false);
						envMapObj->setUniform("u_EnableRefraction", true);
						envMapObj->setUniform("u_EnableDynamicEnvMap", false);
						envMapObj->setUniform<float>("u_RefractionRatio", 1.0f / m_refractionRatio);
						break;
					case 3:
						envMapObj->setUniform("u_EnableDynamicEnvMap", true);
						envMapObj->setUniform("u_EnableReflection", false);
						envMapObj->setUniform("u_EnableRefraction", false);
						break;
					case 0:
					default:
						envMapObj->setUniform("u_EnableDynamicEnvMap", false);
						envMapObj->setUniform("u_EnableReflection", false);
						envMapObj->setUniform("u_EnableRefraction", false);
						break;
				}
			}
			else
			{
				bool hasTextures = !envMapObj->getModelData()->pCustom->texturesLoaded.empty();
				envMapObj->setUniform("u_HasTextures", hasTextures);
				if (!hasTextures)
				{
					envMapObj->setUniform("u_Material.ambient", envMapObj->getBasicMaterial().ambient);
					envMapObj->setUniform("u_Material.diffuse", envMapObj->getBasicMaterial().diffuse);
					envMapObj->setUniform("u_Material.specular", envMapObj->getBasicMaterial().specular);
					envMapObj->setUniform("u_Material.shininess", envMapObj->getBasicMaterial().shininess);
				}
			}
		}


		TestMultipleLights::onUpdate(deltaTime, hardwareInput);
		for (auto& pLight : m_pLights)
		{
			pLight->updateUniforms(m_pEnvMapObjects);
		}
	}

	void TestEnvMapping::onRender()
	{
		// 绘制动态环境贴图的内容
		if (m_envMappingParam == 3)
		{
			// 保留原来的摄像机
			std::unique_ptr<GLCore::Camera> originCam;
			originCam.swap(m_pCamera);

			for (int i = 0; i < 6; ++i)
			{
				GLCore::Camera camera;
				glm::vec2 rotations[] =
				{
					glm::vec2(180.0f, 90.0f),	// 右
					glm::vec2(180.0f, -90.0f),	// 左
					glm::vec2(90.0f, 0.0f),		// 上 +y
					glm::vec2(-90.0f, 0.0f),		// 下 
					glm::vec2(180.0f, 0.0f),		// 前 +z
					glm::vec2(180.0f, 180.0f),	// 后 -z
				};
				m_pCamera = std::make_unique<GLCore::Camera>(camera);
				m_pCamera->setPitch(rotations[i].x);
				m_pCamera->setYaw(rotations[i].y);
				m_pCamera->setFov(90.0f);
				m_pCamera->updateCameraVectors();
				m_view = m_pCamera->getViewMat();
				m_proj = m_pCamera->getPerspectiveProjMat(1024, 1024);
				onUpdate(0.0f, {});

				// 渲染场景(除了要反射的物体)
				m_pDynEnvMapFBO->updateCubeMapTexAttachment(m_pDynamicEnvMap->getID(), GL_TEXTURE_CUBE_MAP_POSITIVE_X + i);
				
				m_pDynEnvMapFBO->bindFBO();
				m_pDynamicEnvMap->bind(7);

				//GLCall(glEnable(GL_DEPTH_TEST));
				GLCall(glViewport(0, 0, 1024, 1024));
				GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
				GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

				TestMultipleLights::onRender();
				{
					GLCore::Renderer renderer(nullptr);
					for (auto& envMapObj : m_pEnvMapObjects)
					{
						if (envMapObj->getDataType() != GLCore::ModelDataType::RAW)
							envMapObj->onRender(renderer);
					}
				}
			}

			m_pDynEnvMapFBO->unbindFBO();
			m_pDynamicEnvMap->unbind();

			m_pCamera.swap(originCam);
			onUpdate(0.0f, {});
		}

		GLCall(glViewport(0, 0, m_testWindowWidth, m_testWindowHeight));
		TestMultipleLights::onRender();
		{
			GLCore::Renderer renderer(nullptr);
			for (auto& envMapObj : m_pEnvMapObjects)
			{
				if (m_envMappingParam == 3)
				{
					m_pDynamicEnvMap->bind(7);
					envMapObj->setUniform("u_Skybox", 7);
					envMapObj->onRender(renderer);
				}
				else
				{
					envMapObj->onRender(renderer);
				}
			}
		}
	}

	void TestEnvMapping::onImGuiRender()
	{
		ImGui::SeparatorText("环境映射测试##TestEnvMapping");
		if (ImGui::Button("无效果##TestEnvMapping"))
		{
			m_envMappingParam = 0;
		}
		if (ImGui::Button("反射##TestEnvMapping"))
		{
			m_envMappingParam = 1;
		}
		if (ImGui::Button("折射##TestEnvMapping"))
		{
			m_envMappingParam = 2;
		}
		ImGui::DragFloat("折射率##TestEnvMapping", &m_refractionRatio, 0.005f, 1.0f, 3.0f);
		if (ImGui::Button("动态环境映射##TestEnvMapping"))
		{
			m_envMappingParam = 3;
		}
		if (ImGui::Button("创建测试用立方体##TestEnvMapping"))
		{
			m_pEnvMapObjects.push_back(std::make_unique<GLCore::GLObject>(vertices, sizeof(vertices),
								    GLCore::GLVertexBufferLayout({3, 3}),
									std::string(proj_res_path + "/Shaders/TestEnvMapping/object.vert"),
									std::string(proj_res_path + "/Shaders/TestEnvMapping/object.frag")));
		}
		if (ImGui::Button("创建测试用NanoSuit模型##TestEnvMapping"))
		{
			std::string filePath(proj_res_path + "/Models/nanosuit_2/nanosuit.obj");
			std::replace(filePath.begin(), filePath.end(), '/', '\\');
			m_pEnvMapObjects.push_back(std::make_unique<GLCore::GLObject>(
				filePath,
				std::string(proj_res_path + "/Shaders/CustomModel/model.vert"),
				std::string(proj_res_path + "/Shaders/TestEnvMapping/model.frag")));
		}

		TestMultipleLights::onImGuiRender();
		// 补充内容
		ImGui::Begin("物体##TestMultipleLights");
		for (size_t i = 0; i < m_pEnvMapObjects.size(); ++i)
		{
			std::string name = m_pEnvMapObjects.at(i)->getDataType() == GLCore::ModelDataType::RAW ? "Env立方体" : "Env自定义模型";
			m_pEnvMapObjects.at(i)->onImGuiRender(name);
			if (ImGui::Button(std::string("删除该模型##" + m_pEnvMapObjects.at(i)->getUUID()).c_str()))
			{
				m_pEnvMapObjects.at(i).reset();
				m_pEnvMapObjects.erase(std::begin(m_pEnvMapObjects) + i);
			}
		}
		ImGui::End();
	}
}
