#include "Test/TestBlinnPhong.h"

namespace test
{
	TestBlinnPhong::TestBlinnPhong()
	{
		// 启用混合和深度缓冲
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		GLCall(glEnable(GL_DEPTH_TEST));

		// Cube信息
		// 嫌麻烦，把EBO删了
		float vertices[] = {
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
			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
		};

		const std::string proj_res_path(PROJ_RES_PATH);
		m_pCamera = std::make_unique<GLCore::Camera>(glm::vec3(0.0f, 0.0f, 3.0f));
		m_pWoodBox = std::make_unique<GLCore::GLObject>(vertices, sizeof(vertices),
		                                                GLCore::GLVertexBufferLayout({3, 3}),
		                                                std::string(proj_res_path + "/Shaders/TestBlinnPhong/object.vert"),
		                                                std::string(proj_res_path + "/Shaders/TestBlinnPhong/object.frag"));
		m_pLight = std::make_unique<GLCore::Light>(glm::vec3(1.0f), glm::vec3(0.7f, 0.7f, 1.5f));

		m_enableAmbientLight = true;
		m_enableDiffuseLight = true;
		m_enableSpecularLight = true;
	}

	TestBlinnPhong::~TestBlinnPhong()
	{
		// 关闭混合和深度缓冲
		GLCall(glDisable(GL_BLEND));
		GLCall(glDisable(GL_DEPTH_TEST));

		m_pCamera.reset();
		m_pWoodBox.reset();
	}

	void TestBlinnPhong::onUpdate(float deltaTime, const Input& hardwareInput)
	{
		// 处理硬件输入
		processInput(hardwareInput, deltaTime);

		// 处理Shader
		glm::mat4 model = m_pWoodBox->getModelMat();
		m_view = m_pCamera->getViewMat();
		m_proj = glm::perspective(glm::radians(m_pCamera->getCameraFOV()),
								  static_cast<float>(hardwareInput.screenWidth) / static_cast<float>(hardwareInput.screenHeight),
								  0.1f, 100.0f);
		glm::mat3 normalMat = glm::transpose(glm::inverse(glm::mat3(model)));

		m_pWoodBox->setUniform("u_MVP", m_proj * m_view * model);
		m_pWoodBox->setUniform("u_Model", model);
		m_pWoodBox->setUniform("u_Normal", normalMat);
		m_pWoodBox->setUniform("u_Color", m_pWoodBox->getColor());
		m_pWoodBox->setUniform("u_LightColor", m_pLight->getColor());
		m_pWoodBox->setUniform("u_LightPos", m_pLight->getTranslation());
		m_pWoodBox->setUniform("u_CameraPos", m_pCamera->getCameraPos());
		m_pWoodBox->setUniform("u_EnableAmbient", m_enableAmbientLight);
		m_pWoodBox->setUniform("u_EnableDiffuse", m_enableDiffuseLight);
		m_pWoodBox->setUniform("u_EnableSpecular", m_enableSpecularLight);

		model = m_pLight->getModelMat();
		m_pLight->setUniform("u_MVP", m_proj * m_view * model);
		m_pLight->setUniform("u_LightColor", m_pLight->getColor());
	}

	void TestBlinnPhong::onRender()
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		{
			GLCore::Renderer renderer(nullptr);
			m_pWoodBox->onRender(renderer);
			m_pLight->onRender(renderer);
		}
	}

	void TestBlinnPhong::onImGuiRender()
	{
		ImGui::Checkbox("Enable Ambient##TestBlinnPhong", &m_enableAmbientLight);
		ImGui::Checkbox("Enable Diffuse##TestBlinnPhong", &m_enableDiffuseLight);
		ImGui::Checkbox("Enable Specular##TestBlinnPhong", &m_enableSpecularLight);

		ImGui::Begin("Objects##TestBlinnPhong");
		m_pWoodBox->onImGuiRender("WoodBox");
		m_pLight->onImGuiRender("Light");
		ImGui::End();
	}

	void TestBlinnPhong::processInput(const Input& hardware_input, float deltaTime) const
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
	}
}
