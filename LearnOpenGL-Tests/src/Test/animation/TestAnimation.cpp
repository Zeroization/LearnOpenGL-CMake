#include "Test/animation/TestAnimation.h"

#include "Render/Light/DirectionalLight.h"
#include "Render/Light/PointLight.h"
#include "Render/Light/SpotLight.h"

#include "thirdParty/FileBrowser/ImGuiFileDialog.h"
#include "thirdParty/FileBrowser/ImGuiFileDialogConfig.h"

namespace test
{
	static float cubeVertices[] = {
		// positions          
		-0.5f, -0.5f, -0.5f,  
		 0.5f, -0.5f, -0.5f,  
		 0.5f,  0.5f, -0.5f,  
		 0.5f,  0.5f, -0.5f,  
		-0.5f,  0.5f, -0.5f,  
		-0.5f, -0.5f, -0.5f,  

		-0.5f, -0.5f,  0.5f,  
		 0.5f, -0.5f,  0.5f,  
		 0.5f,  0.5f,  0.5f,  
		 0.5f,  0.5f,  0.5f, 
		-0.5f,  0.5f,  0.5f,  
		-0.5f, -0.5f,  0.5f, 

		-0.5f,  0.5f,  0.5f, 
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,

		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f, 
		 0.5f, -0.5f, -0.5f,  
		 0.5f, -0.5f,  0.5f, 
		 0.5f,  0.5f,  0.5f, 

		-0.5f, -0.5f, -0.5f,  
		 0.5f, -0.5f, -0.5f,  
		 0.5f, -0.5f,  0.5f,  
		 0.5f, -0.5f,  0.5f,  
		-0.5f, -0.5f,  0.5f,  
		-0.5f, -0.5f, -0.5f,  

		-0.5f,  0.5f, -0.5f, 
		 0.5f,  0.5f, -0.5f,  
		 0.5f,  0.5f,  0.5f,  
		 0.5f,  0.5f,  0.5f, 
		-0.5f,  0.5f,  0.5f,  
		-0.5f,  0.5f, -0.5f,
	};

	static std::string proj_res_path(PROJ_RES_PATH);

	TestAnimation::TestAnimation()
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

		m_pObjects.push_back(std::make_unique<GLCore::GLObject>(
			// std::string("D:\\PROGRAMMING\\Dev\\cpp\\LearnOpenGL-CMake\\Res\\Models\\Woman\\Woman.gltf"),
			 std::string("D:\\PROGRAMMING\\Dev\\cpp\\LearnOpenGL-CMake\\Res\\Models\\Aru\\aru.gltf"),
			std::string(proj_res_path + "/Shaders/CustomModel/Animation/model.vert")));

		// TODO: 目前是Two-Bone IK的配置
		// 0 - root, 1 - middle, 2 - effector
		for (int i = 0; i < 3; ++i)
		{
			m_pCubesForIK.push_back(std::make_unique<GLCore::GLObject>(
				cubeVertices, sizeof(cubeVertices), GLCore::GLVertexBufferLayout({3}),
				std::string(proj_res_path + "/Shaders/Light/light.vert"),
				std::string(proj_res_path + "/Shaders/Light/light.frag")
			));
		}

		m_pTargetForIk = std::make_unique<GLCore::GLObject>(
			cubeVertices, sizeof(cubeVertices), GLCore::GLVertexBufferLayout({3}),
			std::string(proj_res_path + "/Shaders/Light/light.vert"),
			std::string(proj_res_path + "/Shaders/Light/light.frag")
		);
	}

	TestAnimation::~TestAnimation()
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

	void TestAnimation::onUpdate(float deltaTime, const Input& hardwareInput)
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

			m_pObject->SetEnableAnimation(m_isEnableAnimation);
			m_pObject->SetEnableAnimLerpBlend(m_isEnableLerpBlending);
			m_pObject->SetEnableAnimCrossfadeBlend(m_isEnableCrossFadeBlending);
			m_pObject->SetEnableAnimPartialBlend(m_isEnablePartialBlending);
			m_pObject->SetAnimMaskJointNameList({"Bip001 L Thigh", "Bip001 R Thigh"});
			m_pObject->SetEnableAnimAdditiveBlend(m_isEnableAdditiveBlending);
			m_pObject->SetAnimSrcRefClipForAdditiveBlend("Normal_Reload", "Normal_Idle");

			if (m_isEnableAnimation && m_ikCurOpt != 0)
			{
				m_pObject->SetAnimIKOpt(m_ikCurOpt);
				static glm::vec3 tmpPos = glm::vec3(114514.0f);
				if (tmpPos != m_ikTargetPos)
				{
					tmpPos = m_ikTargetPos;
					m_pObject->SetTwoBoneIkParams("Bip001 L Foot", m_ikTargetPos);
				}
			}

			m_pObject->onUpdate(deltaTime);
		}

		for (const auto& m_pLight : m_pLights)
		{
			glm::mat4 model = m_pLight->getModelMat();
			m_pLight->setUniform("u_MVP", m_proj * m_view * model);
			m_pLight->setUniform("u_LightColor", m_pLight->getBasicMaterial()->diffuse);

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

		// Target for Ik
		m_pTargetForIk->setScale(glm::vec3(0.05f));
		m_pTargetForIk->setUniform("u_MVP", m_proj * m_view * m_pTargetForIk->getModelMat());
		m_pTargetForIk->setUniform("u_LightColor", glm::vec3(1.f, 0.f, 0.f));

		// Cubes for IK
		const std::array<std::string, 3> boneNames = {"Bip001 L Thigh", "Bip001 L Calf", "Bip001 L Foot"};
		for (size_t i = 0; i < m_pCubesForIK.size(); ++i)
		{
			// TODO: 暂时是Two-bones的配置
			assert(i < 3);

			glm::mat4 cubeModelMat = glm::scale(m_pObjects.at(0)->getModelMat() *
												m_pObjects.at(0)->GetBoneMatByName(boneNames[i]),
												glm::vec3(0.025f));
			m_pCubesForIK.at(i)->setUniform("u_MVP", m_proj * m_view * cubeModelMat);
			m_pCubesForIK.at(i)->setUniform("u_LightColor", glm::vec3(0.f, 1.f, 0.f));
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
			m_pTargetForIk->onRender(renderer);

			GLCall(glDisable(GL_DEPTH_TEST));
			for (auto& m_pIkCube : m_pCubesForIK)
			{
				m_pIkCube->onRender(renderer);
			}
			GLCall(glEnable(GL_DEPTH_TEST));

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

		ImGui::SeparatorText("Settings##TestAnimation");
		ImGui::Checkbox("Enable Animation##TestAnimation", &m_isEnableAnimation);
		if (m_isEnableAnimation)
		{
			ImGui::SeparatorText("Animation Blending##TestAnimaiton");
			ImGui::Checkbox("Enable Simple LERP Blend##TestAnimation", &m_isEnableLerpBlending);
			ImGui::Checkbox("Enable CrossFading Blend##TestAnimation", &m_isEnableCrossFadeBlending);
			ImGui::Checkbox("Enable Partial Blend##TestAnimation", &m_isEnablePartialBlending);
			ImGui::Checkbox("Enable Additive Blend##TestAnimation", &m_isEnableAdditiveBlending);

			ImGui::SeparatorText("Animation IK##TestAnimaiton");
			ImGui::Checkbox("Enable IK##TestAnimation", &m_isEnableIK);
			if (m_isEnableIK)
			{
				ImGui::Combo("Select IK", &m_ikCurOpt, "None\0Two-Bone IK\0CCD IK\0FABRIK\0\0");
				ImGui::DragFloat3("Target Pos##TestAnimation", &m_ikTargetPos.x, 0.05f, -5.0f, 5.0f);
				m_pTargetForIk->setTranslation(m_ikTargetPos);
			}
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
