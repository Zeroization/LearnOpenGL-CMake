#include "Test/TestMultipleLights.h"

#include "Render/Light/DirectionalLight.h"
#include "Render/Light/PointLight.h"
#include "Render/Light/SpotLight.h"

#include "thirdParty/FileBrowser/ImGuiFileDialog.h"
#include "thirdParty/FileBrowser/ImGuiFileDialogConfig.h"

namespace test
{
	static float vertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	};

	static std::string proj_res_path(PROJ_RES_PATH);

	TestMultipleLights::TestMultipleLights()
	{
		// 启用混合
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		// 启用深度缓冲
		GLCall(glEnable(GL_DEPTH_TEST));

		m_pCamera = std::make_unique<GLCore::Camera>(glm::vec3(0.0f, 0.0f, 3.0f));
		m_pSkybox = std::make_unique<GLCore::SkyBox>(std::vector<GLCore::CubeMapTexDesc>({
			{std::string(proj_res_path + "/Textures/skybox/sky/back.jpg") , GLCore::CubeMapTexDir::Back, false},
			{std::string(proj_res_path + "/Textures/skybox/sky/bottom.jpg") , GLCore::CubeMapTexDir::Bottom, false},
			{std::string(proj_res_path + "/Textures/skybox/sky/front.jpg") , GLCore::CubeMapTexDir::Front, false},
			{std::string(proj_res_path + "/Textures/skybox/sky/left.jpg") , GLCore::CubeMapTexDir::Left, false},
			{std::string(proj_res_path + "/Textures/skybox/sky/right.jpg") , GLCore::CubeMapTexDir::Right, false},
			{std::string(proj_res_path + "/Textures/skybox/sky/top.jpg") , GLCore::CubeMapTexDir::Top, false}
		}));
	}

	TestMultipleLights::~TestMultipleLights()
	{
		// 关闭混合和深度缓冲
		GLCall(glDisable(GL_BLEND));
		GLCall(glDisable(GL_DEPTH_TEST));

		m_pCamera.reset();
		for (auto& p_object : m_pObjects)
		{
			p_object.reset();
		}
		for (auto& p_light : m_pLights)
		{
			p_light.reset();
		}
	}

	void TestMultipleLights::onUpdate(float deltaTime, const Input& hardwareInput)
	{
		// 处理硬件输入
		processInput(hardwareInput, deltaTime);

		// 处理Shader
		m_view = m_pCamera->getViewMat();
		m_proj = m_pCamera->getPerspectiveProjMat(hardwareInput.screenWidth, hardwareInput.screenHeight);

		for (const auto& m_pObject : m_pObjects)
		{
			glm::mat4 model = m_pObject->getModelMat();
			glm::mat3 normalMat = glm::transpose(glm::inverse(glm::mat3(model)));

			m_pObject->setUniform("u_MVP", m_proj * m_view * model);
			m_pObject->setUniform("u_Model", model);
			m_pObject->setUniform("u_Normal", normalMat);
			m_pObject->setUniform("u_CameraPos", m_pCamera->getCameraPos());
			if (m_pObject->getDataType() == GLCore::ModelDataType::RAW)
			{
				m_pObject->setUniform("u_Material.diffuse", 0);
				m_pObject->setUniform("u_Material.specular", 1);
				m_pObject->setUniform("u_Material.shininess", m_pObject->getBasicMaterial().shininess);
			}
			else
			{
				m_pObject->setUniform("u_HasTextures", !m_pObject->getModelData()->pCustom->texturesLoaded.empty());
				m_pObject->setUniform("u_Material.ambient", m_pObject->getBasicMaterial().ambient);
				m_pObject->setUniform("u_Material.diffuse", m_pObject->getBasicMaterial().diffuse);
				m_pObject->setUniform("u_Material.specular", m_pObject->getBasicMaterial().specular);
				m_pObject->setUniform("u_Material.shininess", m_pObject->getBasicMaterial().shininess);
			}
		}

		for (const auto& m_pLight : m_pLights)
		{
			glm::mat4 model = m_pLight->getModelMat();
			m_pLight->setUniform("u_MVP", m_proj * m_view * model);
			m_pLight->setUniform("u_LightColor", m_pLight->getBasicMaterial().diffuse);

			// 处理聚光灯和平行光的pArrow
			if (m_pLight->getLightType() == GLCore::LightType::DirectionalLight)
			{
				GLCore::DirectionalLight* dirLight = dynamic_cast<GLCore::DirectionalLight*>(m_pLight.get());
				GLCore::GLObject* pArrow = dirLight->getArrow();

				pArrow->setRotation(m_pLight->getRotation());
				pArrow->setTranslation(m_pLight->getTranslation());
				model = pArrow->getModelMat();
				pArrow->setUniform("u_MVP", m_proj * m_view * model);
				pArrow->setUniform("u_Color", pArrow->getColor());
			}
			else if (m_pLight->getLightType() == GLCore::LightType::SpotLight)
			{
				GLCore::SpotLight* spotLight = dynamic_cast<GLCore::SpotLight*>(m_pLight.get());
				GLCore::GLObject* pArrow = spotLight->getArrow();

				pArrow->setRotation(m_pLight->getRotation());
				pArrow->setTranslation(m_pLight->getTranslation());
				model = pArrow->getModelMat();
				pArrow->setUniform("u_MVP", m_proj * m_view * model);
				pArrow->setUniform("u_Color", pArrow->getColor());
			}

			// 处理每个物体
			m_pLight->updateUniforms(m_pObjects);
		}

		// Skybox
		m_view = glm::mat4(glm::mat3(m_pCamera->getViewMat()));
		m_pSkybox->setUniform("u_VP", m_proj * m_view);
		m_pSkybox->setUniform("u_Skybox", 0);
	}

	void TestMultipleLights::onRender()
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		{
			GLCore::Renderer renderer(nullptr);
			for (auto& m_pObject : m_pObjects)
			{
				m_pObject->onRender(renderer);
			}
			for (auto& m_pLight : m_pLights)
			{
				m_pLight->onRender(renderer);
			}

			if (m_selfSkyboxRender)	renderSkybox(renderer);
		}
	}

	void TestMultipleLights::renderSkybox(const GLCore::Renderer& renderer) const
	{
		GLCall(glDepthFunc(GL_LEQUAL));
		m_pSkybox->onRender(renderer);
		GLCall(glDepthFunc(GL_LESS));
	}

	void TestMultipleLights::onImGuiRender()
	{
		ImGui::SeparatorText("Render Target##TestFrameBuffer");
		if (ImGui::Button("Wooden Box##TestMultipleLights"))
		{
			m_pObjects.push_back(std::make_unique<GLCore::GLObject>(vertices, sizeof(vertices),
								   GLCore::GLVertexBufferLayout({3, 3, 2}),
						   std::string(proj_res_path + "/Shaders/TestMultipleLights/object.vert"),
					       std::string(proj_res_path + "/Shaders/TestMultipleLights/object.frag"),
				     std::vector<GLCore::TextureDesc>({
				     	{proj_res_path + "/Textures/container2.png", GLCore::TextureType::DiffuseMap, true},
				     	{proj_res_path + "/Textures/container2_specular.png", GLCore::TextureType::SpecularMap, true}
				     })));
		}

		if (ImGui::Button("Model##TestMultipleLights"))
		{
			IGFD::FileDialogConfig config;
			config.path = std::string(PROJ_RES_PATH);
			ImGuiFileDialog::Instance()->OpenDialog("ChooseAModelFile##TestMultipleLights", "Please select model file", ".*", config);
		}
		if (ImGuiFileDialog::Instance()->Display("ChooseAModelFile##TestMultipleLights")) {
			if (ImGuiFileDialog::Instance()->IsOk()) { // action if OK
				std::string filePath = ImGuiFileDialog::Instance()->GetFilePathName();
				m_pObjects.push_back(std::make_unique<GLCore::GLObject>(filePath));
			}
			// close
			ImGuiFileDialog::Instance()->Close();
		}

		if (ImGui::Button("Directional Light##TestMultipleLights"))
		{
			m_pLights.push_back(std::make_unique<GLCore::DirectionalLight>(
				glm::vec3(0.5f), glm::vec3(0.0f, 0.5f, 2.5f)
			));
		}

		if (ImGui::Button("Point Light##TestMultipleLights"))
		{
			m_pLights.push_back(std::make_unique<GLCore::PointLight>());
		}

		if (ImGui::Button("Spot Light##TestMultipleLights"))
		{
			m_pLights.push_back(std::make_unique<GLCore::SpotLight>());
		}

		ImGui::Begin("Objects##TestMultipleLights");
		for (size_t i = 0; i < m_pObjects.size(); ++i)
		{
			std::string name = m_pObjects.at(i)->getDataType() == GLCore::ModelDataType::RAW ? "Wooden Box" : "Model";
			m_pObjects.at(i)->onImGuiRender(name);
			if (ImGui::Button(std::string("Delete##" + m_pObjects.at(i)->getUUID()).c_str()))
			{
				m_pObjects.at(i).reset();
				m_pObjects.erase(std::begin(m_pObjects) + i);
			}
		}
		ImGui::End();

		ImGui::Begin("Lights##TestMultipleLights");
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

	void TestMultipleLights::processInput(const Input& hardware_input, float deltaTime) const
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
