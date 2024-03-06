#include "Test/TestCamera.h"

#include "pch.hpp"
#include "Renderer.h"
#include "Shader.h"

#include "OpenGL/glVertexBufferLayout.hpp"

namespace test
{
	TestCamera::TestCamera()
		: m_translation(glm::vec3(0.0f)), m_color(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f))
	{
		// 启用混合和深度缓冲
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		GLCall(glEnable(GL_DEPTH_TEST));
	
		// 定义摄像机
		mp_camera = std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, 3.0f));
	
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
		mp_VBO = std::make_unique<GLVertexBuffer>(vertices, sizeof(vertices));
		GLVertexBufferLayout layout;
		layout.push<float>(3);
		layout.push<float>(2);
		mp_VAO = std::make_unique<GLVertexArray>();
		mp_VAO->addVBO(*mp_VBO, layout);
		mp_IBO = std::make_unique<GLIndexBuffer>(indices, 36);
	
		// 绑定Shader
		const std::string proj_res_path(PROJ_RES_PATH);
		mp_shader = std::make_unique<Shader>(
			std::string(proj_res_path + "/Shaders/TestCamera/shader.vert"),
			std::string(proj_res_path + "/Shaders/TestCamera/shader.frag")
		);
	}
	
	TestCamera::~TestCamera()
	{
		// 关闭混合和深度缓冲
		GLCall(glDisable(GL_BLEND));
		GLCall(glDisable(GL_DEPTH_TEST));

		mp_shader.reset();
		mp_VAO.reset();
		mp_VBO.reset();
		mp_IBO.reset();
	}
	
	void TestCamera::onUpdate(float deltaTime, unsigned keyboardInput)
	{
		processInput(keyboardInput, deltaTime);

		// 处理Shader
		glm::mat4 model(glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(2.0f, 2.0f, 2.0f)), m_translation));
		m_view = mp_camera->getViewMat();
		m_proj = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
		mp_shader->bind();
		mp_shader->setUniformMat4f("u_MVP", m_proj * m_view * model);
		mp_shader->setUniform4f("u_Color", m_color.r, m_color.g, m_color.b, m_color.a);
		mp_shader->unbind();
	}
	
	void TestCamera::onRender()
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	
		{
			Renderer renderer(nullptr);
			renderer.draw(*mp_VAO, *mp_IBO, *mp_shader);
		}
	}
	
	void TestCamera::onImGuiRender()
	{
		ImGui::SliderFloat3("Cube Position", &m_translation.x, -100.0f, 100.0f);
	}
	
	void TestCamera::processInput(const unsigned keyboardInput, float deltaTime) const
	{
		if (keyboardInput == GLFW_KEY_W)
		{
			// LOG_DEBUG("[Test::Camera] pressed W");
			mp_camera->processKeyboard(Camera::CameraMovDir::FORWARD, deltaTime);
		}
		if (keyboardInput == GLFW_KEY_S)
		{
			// LOG_DEBUG("[Test::Camera] pressed S");
			mp_camera->processKeyboard(Camera::CameraMovDir::BACKWARD, deltaTime);
		}
		if (keyboardInput == GLFW_KEY_A)
		{
			// LOG_DEBUG("[Test::Camera] pressed A");
			mp_camera->processKeyboard(Camera::CameraMovDir::LEFT, deltaTime);
		}
		if (keyboardInput == GLFW_KEY_D)
		{
			// LOG_DEBUG("[Test::Camera] pressed D");
			mp_camera->processKeyboard(Camera::CameraMovDir::RIGHT, deltaTime);
		}
	}
}
