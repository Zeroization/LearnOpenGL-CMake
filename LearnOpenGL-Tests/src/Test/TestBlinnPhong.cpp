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
		float vertices[] = {
			-0.5f, -0.5f, -0.5f, // 0
			0.5f, -0.5f, -0.5f, // 1
			0.5f, 0.5f, -0.5f, // 2
			-0.5f, 0.5f, -0.5f, // 3
			-0.5f, -0.5f, 0.5f, // 4
			0.5f, -0.5f, 0.5f, // 5
			0.5f, 0.5f, 0.5f, // 6
			-0.5f, 0.5f, 0.5f // 7
		};
		unsigned int indices[] = {
			// 注意索引从0开始! 
			0, 1, 2, 2, 3, 0,
			4, 5, 6, 6, 7, 4,
			7, 3, 0, 0, 4, 7,
			6, 2, 1, 1, 5, 6,
			0, 1, 5, 5, 4, 0,
			3, 2, 6, 6, 7, 3
		};

		const std::string proj_res_path(PROJ_RES_PATH);
		m_pCamera = std::make_unique<Camera>();
		m_pWoodBox = std::make_unique<GLObject>(vertices, sizeof(vertices), GLVertexBufferLayout({3}),
											 indices, 36,
											 std::string(proj_res_path + "/Shaders/TestBlinnPhong/object.vert"),
											 std::string(proj_res_path + "/Shaders/TestBlinnPhong/object.frag"));
		m_pLight = std::make_unique<Light>();
		
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

		m_pWoodBox->setUniform("u_MVP", m_proj * m_view * model);
		m_pWoodBox->setUniform("u_Color", glm::vec4(1.0f, 0.5f, 0.31f, 1.0f));

		model = m_pLight->getModelMat();
		m_pLight->setUniform("u_MVP", m_proj * m_view * model);
	}

	void TestBlinnPhong::onRender()
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		{
			Renderer renderer(nullptr);
			m_pWoodBox->onRender(renderer);
			m_pLight->onRender(renderer);
		}
	}

	void TestBlinnPhong::onImGuiRender()
	{
		m_pWoodBox->onImGuiRender("WoodBox");
		m_pLight->onImGuiRender("Light");
	}

	void TestBlinnPhong::processInput(const Input& hardware_input, float deltaTime) const
	{
		// 鼠标
		m_pCamera->processMouse(hardware_input.mouseMovXOffset, hardware_input.mouseMovYOffset,
		                        hardware_input.mouseScrollYOffset);

		// 键盘
		if (hardware_input.keyboardInput == GLFW_KEY_W)
		{
			m_pCamera->processKeyboard(Camera::CameraMovDir::FORWARD, deltaTime);
		}
		if (hardware_input.keyboardInput == GLFW_KEY_S)
		{
			m_pCamera->processKeyboard(Camera::CameraMovDir::BACKWARD, deltaTime);
		}
		if (hardware_input.keyboardInput == GLFW_KEY_A)
		{
			m_pCamera->processKeyboard(Camera::CameraMovDir::LEFT, deltaTime);
		}
		if (hardware_input.keyboardInput == GLFW_KEY_D)
		{
			m_pCamera->processKeyboard(Camera::CameraMovDir::RIGHT, deltaTime);
		}
	}
}
