#include "Test/TestCamera.h"

#include "pch.hpp"
#include "Core/Renderer.h"

namespace test
{
	TestCamera::TestCamera()
		: m_translation(glm::vec3(0.0f, 0.0f, -4.0f)), m_color(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f))
	{
		// 定义正方体顶点信息
		float vertices[] = {
			-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
			0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
			0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
			-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
			-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
			0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
			0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
			-0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
			-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
			-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
			0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
			0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
			0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
			0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
			-0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
			-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
			-0.5f, 0.5f, -0.5f, 1.0f, 1.0f
		};
		unsigned int indices[] = {
			// 注意索引从0开始! 
			0, 1, 2, 2, 3, 0,
			4, 5, 6, 6, 7, 4,
			8, 16, 9, 9, 4, 8,
			10, 2, 11, 11, 12, 10,
			9, 13, 5, 5, 4, 9,
			3, 2, 10, 10, 14, 15
		};

		// 定义摄像机
		m_pCamera = std::make_unique<Camera>();
		const std::string proj_res_path(PROJ_RES_PATH);
		m_pCube = std::make_unique<GLObject>(vertices, sizeof(vertices), GLVertexBufferLayout({3, 2}),
											 indices, 36, 
											 std::string(proj_res_path + "/Shaders/TestCamera/shader.vert"),
											 std::string(proj_res_path + "/Shaders/TestCamera/shader.frag"));

		// 启用混合和深度缓冲
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		GLCall(glEnable(GL_DEPTH_TEST));
	}
	
	TestCamera::~TestCamera()
	{
		// 关闭混合和深度缓冲
		GLCall(glDisable(GL_BLEND));
		GLCall(glDisable(GL_DEPTH_TEST));

		m_pCamera.reset();
		m_pCube.reset();
	}
	
	void TestCamera::onUpdate(float deltaTime, const Input& hardwareInput)
	{
		// 处理硬件输入
		processInput(hardwareInput, deltaTime);

		// 处理Shader
		glm::mat4 model = m_pCube->getModelMat();
		m_view = m_pCamera->getViewMat();
		m_proj = glm::perspective(glm::radians(m_pCamera->getCameraFOV()),
								  static_cast<float>(hardwareInput.screenWidth) / static_cast<float>(hardwareInput.screenHeight),
								  0.1f, 100.0f);

		m_pCube->setUniform("u_MVP", m_proj * m_view * model);
		m_pCube->setUniform("u_Color", m_color);
	}
	
	void TestCamera::onRender()
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	
		{
			Renderer renderer(nullptr);
			m_pCube->onRender(renderer);
		}
	}
	
	void TestCamera::onImGuiRender()
	{
		m_pCube->onImGuiRender("Cube");
	}
	
	void TestCamera::processInput(const Input& hardware_input, float deltaTime) const
	{
		// 鼠标
		m_pCamera->processMouse(hardware_input.mouseMovXOffset, hardware_input.mouseMovYOffset, hardware_input.mouseScrollYOffset);

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
