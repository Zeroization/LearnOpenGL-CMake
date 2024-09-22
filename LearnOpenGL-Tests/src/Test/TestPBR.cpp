#include "Test/TestPBR.h"

#include "thirdParty/FileBrowser/ImGuiFileDialog.h"
#include "thirdParty/FileBrowser/ImGuiFileDialogConfig.h"

#include "Render/Light/PointLight.h"

namespace test
{
	static std::string proj_res_path(PROJ_RES_PATH);

	TestPBR::TestPBR()
	{
		GLCall(glEnable(GL_DEPTH_TEST));

		// 摄像机
		m_pCamera = std::make_unique<GLCore::Camera>(glm::vec3(0.0f, 0.0f, 3.0f));

		// 光源
		m_pLights.push_back(std::make_unique<GLCore::PointLight>(
			glm::vec3(300.0f, 300.0f, 300.0f),
			glm::vec3(-10.0f, 10.0f, 10.0f)
		));
		m_pLights.push_back(std::make_unique<GLCore::PointLight>(
			glm::vec3(300.0f, 300.0f, 300.0f),
			glm::vec3(10.0f, 10.0f, 10.0f)
		));
		m_pLights.push_back(std::make_unique<GLCore::PointLight>(
			glm::vec3(300.0f, 300.0f, 300.0f),
			glm::vec3(-10.0f, -10.0f, 10.0f)
		));
		m_pLights.push_back(std::make_unique<GLCore::PointLight>(
			glm::vec3(300.0f, 300.0f, 300.0f),
			glm::vec3(10.0f, -10.0f, 10.0f)
		));

		// PBR材质
		m_pTextures.push_back(std::make_shared<GLCore::GLTexture>(
			std::string(proj_res_path + "/Textures/rusted_iron/albedo.png"),
			GLCore::TextureType::DiffuseMap,
			true
		));
		m_pTextures.push_back(std::make_shared<GLCore::GLTexture>(
			std::string(proj_res_path + "/Textures/rusted_iron/metallic.png"),
			GLCore::TextureType::SpecularMap,
			true
		));
		m_pTextures.push_back(std::make_shared<GLCore::GLTexture>(
			std::string(proj_res_path + "/Textures/rusted_iron/normal.png"),
			GLCore::TextureType::NormalMap,
			true
		));
		m_pTextures.push_back(std::make_shared<GLCore::GLTexture>(
			std::string(proj_res_path + "/Textures/rusted_iron/roughness.png"),
			GLCore::TextureType::SpecularMap,
			true
		));
		m_pTextures.push_back(std::make_shared<GLCore::GLTexture>(
			std::string(proj_res_path + "/Textures/rusted_iron/ao.png"),
			GLCore::TextureType::SpecularMap,
			true
		));

		// PBR球体
		m_nrRows = 7;
		m_nrColumns = 7;
		m_spacing = 2.5;
		for (int row = 0; row < m_nrRows; ++row)
		{
			float metallic = static_cast<float>(row) / static_cast<float>(m_nrRows);
			for (int col = 0; col < m_nrColumns; ++col)
			{
				float roughness = glm::clamp(static_cast<float>(col) / static_cast<float>(m_nrColumns), 0.05f, 1.0f);

				std::string winFilePath = proj_res_path;
				std::replace(winFilePath.begin(), winFilePath.end(), '/', '\\');
				winFilePath += R"(\Models\Simple\sphere.obj)";
				m_pObjects.push_back(std::make_unique<GLCore::GLObject>(
					winFilePath,
					std::string(proj_res_path + "/Shaders/TestPBR/PBRobj.vert"),
					std::string(proj_res_path + "/Shaders/TestPBR/PBRobj.frag")
				));
				m_pObjects.back()->setTranslation(glm::vec3(
					(col - (m_nrColumns / 2)) * m_spacing,
					(row - (m_nrRows / 2)) * m_spacing,
					0.0f
				));
				// m_pObjects.back()->setScale(0.4f, 0.4f, 0.4f);

				m_pObjects.back()->setUniform("enableTexture", m_enableTexture);
				m_pObjects.back()->setUniform("albedoMap", 0);
				m_pObjects.back()->setUniform("metallicMap", 1);
				m_pObjects.back()->setUniform("normalMap", 2);
				m_pObjects.back()->setUniform("roughnessMap", 3);
				m_pObjects.back()->setUniform("aoMap", 4);


				m_pObjects.back()->setUniform("model", m_pObjects.back()->getModelMat());
				m_pObjects.back()->setUniform("normalMatrix",
											  glm::transpose(glm::inverse(glm::mat3(m_pObjects.back()->getModelMat()))));

				m_pObjects.back()->setUniform("metallic", metallic);
				m_pObjects.back()->setUniform("roughness", roughness);
				m_pObjects.back()->setUniform("albedo", glm::vec3(0.5f, 0.0f, 0.0f));
				m_pObjects.back()->setUniform("ao", 1.0f);
			}
		}
	}

	TestPBR::~TestPBR()
	{
		GLCall(glDisable(GL_DEPTH_TEST));
	}

	void TestPBR::onUpdate(float deltaTime, const Input& hardwareInput)
	{
		// 处理硬件输入
		processInput(hardwareInput, deltaTime);

		// 处理Shader
		m_view = m_pCamera->getViewMat();
		m_proj = m_pCamera->getPerspectiveProjMat(1280, 768);

		for (const auto& m_pLight : m_pLights)
		{
			glm::mat4 model = m_pLight->getModelMat();
			m_pLight->setUniform("u_MVP", m_proj * m_view * model);
			m_pLight->setUniform("u_LightColor", m_pLight->getBasicMaterial()->diffuse);
		}

		for (const auto& obj : m_pObjects)
		{
			obj->setUniform("projection", m_proj);
			obj->setUniform("view", m_view);
			obj->setUniform("camPos", m_pCamera->getCameraPos());
			for (int i = 0; i < m_pLights.size(); ++i)
			{	
				obj->setUniform(std::format("lightPositions[{}]", i),
											  m_pLights.at(i)->getTranslation());
				obj->setUniform(std::format("lightColors[{}]", i),
											  m_pLights.at(i)->getBasicMaterial()->diffuse);
			}
		}
	}

	void TestPBR::onRender()
	{
		GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
		{
			GLCore::Renderer renderer(nullptr);
			for (const auto& obj : m_pObjects)
			{
				if (m_enableTexture)
				{
					obj->setUniform("enableTexture", m_enableTexture);
					for (int i = 0; i < 5; ++i)
					{
						m_pTextures.at(i)->bind(i);
					}
				}
				obj->onRender(renderer);
			}
			for (const auto& light : m_pLights)
			{
				light->onRender(renderer);
			}
		}
	}

	void TestPBR::onImGuiRender()
	{
		ImGui::SeparatorText("Toggles##TestPBR");
		ImGui::Checkbox("enable texture##TestPBR", &m_enableTexture);

		ImGui::Begin("Objects##TestPBR");
		for (size_t i = 0; i < m_pObjects.size(); ++i)
		{
			std::string name = "Model";
			m_pObjects.at(i)->onImGuiRender(name);
			if (ImGui::Button(std::string("Delete##" + m_pObjects.at(i)->getUUID()).c_str()))
			{
				m_pObjects.at(i).reset();
				m_pObjects.erase(std::begin(m_pObjects) + i);
			}
		}
		ImGui::End();

		ImGui::Begin("Lights##TestPBR");
		for (size_t i = 0; i < m_pLights.size(); ++i)
		{
			m_pLights.at(i)->onImGuiRender(m_pLights.at(i)->getLightTypeString());
			if (ImGui::Button(std::string("Delete##" + m_pLights.at(i)->getUUID()).c_str()))
			{
				m_pLights.at(i)->releaseUniforms(m_pObjects);
				m_pLights.at(i).reset();
				m_pLights.erase(std::begin(m_pLights) + i);
			}
		}
		ImGui::End();
	}

	void TestPBR::processInput(const Input& hardware_input, float deltaTime) const
	{
		// 鼠标
		m_pCamera->processMouse(hardware_input.mouseMovXOffset, hardware_input.mouseMovYOffset,
								hardware_input.mouseScrollYOffset);
		// 键盘
		if (hardware_input.keyboardInput == GLFW_KEY_W)
		{
			m_pCamera->processKeyboard(GLCore::Camera::CameraMovDir::FORWARD, deltaTime);
		}
		if (hardware_input.keyboardInput == GLFW_KEY_S)
		{
			m_pCamera->processKeyboard(GLCore::Camera::CameraMovDir::BACKWARD, deltaTime);
		}
		if (hardware_input.keyboardInput == GLFW_KEY_A)
		{
			m_pCamera->processKeyboard(GLCore::Camera::CameraMovDir::LEFT, deltaTime);
		}
		if (hardware_input.keyboardInput == GLFW_KEY_D)
		{
			m_pCamera->processKeyboard(GLCore::Camera::CameraMovDir::RIGHT, deltaTime);
		}
		if (hardware_input.keyboardInput == GLFW_KEY_SPACE)
		{
			m_pCamera->processKeyboard(GLCore::Camera::CameraMovDir::UP, deltaTime);
		}
		if (hardware_input.keyboardInput == GLFW_KEY_LEFT_CONTROL)
		{
			m_pCamera->processKeyboard(GLCore::Camera::CameraMovDir::DOWN, deltaTime);
		}
	}
}
