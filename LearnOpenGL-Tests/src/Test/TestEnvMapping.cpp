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

	}

	void TestEnvMapping::onUpdate(float deltaTime, const Input& hardwareInput)
	{
		TestMultipleLights::onUpdate(deltaTime, hardwareInput);

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
				envMapObj->setUniform<float>("u_RefractionRatio", 1.00 / m_refractionRatio);
				switch (m_envMappingParam)
				{
					case 1:
						envMapObj->setUniform("u_EnableReflection", true);
						envMapObj->setUniform("u_EnableRefraction", false);
						break;
					case 2:
						envMapObj->setUniform("u_EnableReflection", false);
						envMapObj->setUniform("u_EnableRefraction", true);
						break;
					case 3:
					case 0:
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

		for (auto& pLight : m_pLights)
		{
			pLight->updateUniforms(m_pEnvMapObjects);
		}
	}

	void TestEnvMapping::onRender()
	{
		TestMultipleLights::onRender();

		{
			GLCore::Renderer renderer(nullptr);
			for (auto& envMapObj : m_pEnvMapObjects)
			{
				envMapObj->onRender(renderer);
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
