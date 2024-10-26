#include "Test/animation/TestAnimation.h"

#include "Render/Light/DirectionalLight.h"
#include "Render/Light/PointLight.h"
#include "Render/Light/SpotLight.h"

#include "thirdParty/FileBrowser/ImGuiFileDialog.h"
#include "thirdParty/FileBrowser/ImGuiFileDialogConfig.h"

namespace test
{
	static std::string proj_res_path(PROJ_RES_PATH);

	TestAnimation::TestAnimation()
	{
		// ���û��
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		// ������Ȼ���
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

		m_pObjects.push_back(std::make_unique<GLCore::GLObject>(
			std::string("D:\\PROGRAMMING\\Dev\\cpp\\LearnOpenGL-CMake\\Res\\Models\\vampire\\dancing_vampire.dae"),
			std::string(proj_res_path + "/Shaders/CustomModel/Animation/model.vert")));
		m_pDanceAnimation = std::make_unique<GLCore::Animation>(std::string(proj_res_path + "/Models/vampire/dancing_vampire.dae"), m_pObjects[0].get());
		m_pAnimator = std::make_unique<GLCore::Animator>(m_pDanceAnimation.get());
	}

	TestAnimation::~TestAnimation()
	{
		// �رջ�Ϻ���Ȼ���
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

	void TestAnimation::onUpdate(float deltaTime, const Input& hardwareInput)
	{
		// ����Ӳ������
		processInput(hardwareInput, deltaTime);

		// ������
		m_pAnimator->UpdateAnimation(deltaTime);

		// ����Shader
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
				m_pObject->setUniform("u_Material.shininess", m_pObject->getBasicMaterial()->shininess);
			}
			else
			{
				m_pObject->setUniform("u_HasTextures", !m_pObject->getModelData()->pCustom->texturesLoaded.empty());
				m_pObject->setUniform("u_Material.ambient", m_pObject->getBasicMaterial()->ambient);
				m_pObject->setUniform("u_Material.diffuse", m_pObject->getBasicMaterial()->diffuse);
				m_pObject->setUniform("u_Material.specular", m_pObject->getBasicMaterial()->specular);
				m_pObject->setUniform("u_Material.shininess", m_pObject->getBasicMaterial()->shininess);
			}

			auto transforms = m_pAnimator->GetFinalBoneMatrices();
			for (int i = 0; i < transforms.size(); ++i)
				m_pObject->setUniform(std::format("finalBonesMatrices[{}]", std::to_string(i)), transforms[i]);
		}

		for (const auto& m_pLight : m_pLights)
		{
			glm::mat4 model = m_pLight->getModelMat();
			m_pLight->setUniform("u_MVP", m_proj * m_view * model);
			m_pLight->setUniform("u_LightColor", m_pLight->getBasicMaterial()->diffuse);

			// ����۹�ƺ�ƽ�й��pArrow
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

			// ����ÿ������
			m_pLight->updateUniforms(m_pObjects);
		}

		// Skybox
		m_view = glm::mat4(glm::mat3(m_pCamera->getViewMat()));
		m_pSkybox->setUniform("u_VP", m_proj * m_view);
		m_pSkybox->setUniform("u_Skybox", 0);
	}

	void TestAnimation::onRender()
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

	void TestAnimation::renderSkybox(const GLCore::Renderer& renderer) const
	{
		GLCall(glDepthFunc(GL_LEQUAL));
		m_pSkybox->onRender(renderer);
		GLCall(glDepthFunc(GL_LESS));
	}

	void TestAnimation::onImGuiRender()
	{
		ImGui::SeparatorText("Render Target##TestAnimation");

		if (ImGui::Button("Directional Light##TestAnimation"))
		{
			m_pLights.push_back(std::make_unique<GLCore::DirectionalLight>(
				glm::vec3(0.5f), glm::vec3(0.0f, 0.5f, 2.5f)
			));
		}

		ImGui::Begin("Objects##TestAnimation");
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

		ImGui::Begin("Lights##TestAnimation");
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

	void TestAnimation::processInput(const Input& hardware_input, float deltaTime) const
	{
		// ���
		m_pCamera->processMouse(hardware_input.mouseMovXOffset, hardware_input.mouseMovYOffset,
								hardware_input.mouseScrollYOffset);

		// ����
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
