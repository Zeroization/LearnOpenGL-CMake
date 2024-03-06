#include "Test\TestTexture2D.h"

#include "pch.hpp"
#include "Renderer.h"
#include "Shader.h"

#include "OpenGL/glVertexBufferLayout.hpp"

#include "imgui/thirdParty/FileBrowser/ImGuiFileDialog.h"
#include "imgui/thirdParty/FileBrowser/ImGuiFileDialogConfig.h"

namespace test
{
	TestTexture2D::TestTexture2D()
		: m_translation(200.0f, 150.0f, 0.0f),
		m_proj(glm::ortho(0.0f, 800.0f, 0.0f, 600.f, -1.0f, 1.0f)),
		m_view(1.0f)
	{
		// 启用混合
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		// 初始化数据
		float positions[] = {
			100.0f, 100.0f, 0.0f, 0.0f, // 0
			200.0f, 100.0f, 1.0f, 0.0f,  // 1
			200.0f, 200.0f, 1.0f, 1.0f,    // 2
			100.0f, 200.0f, 0.0f, 1.0f   // 3
		};

		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		mp_VBO = std::make_unique<GLVertexBuffer>(positions, 4 * 4 * sizeof(float));
		GLVertexBufferLayout layout;
		layout.push<float>(2);
		layout.push<float>(2);

		mp_VAO = std::make_unique<GLVertexArray>();
		mp_VAO->addVBO(*mp_VBO, layout);

		mp_IBO = std::make_unique<GLIndexBuffer>(indices, 6);

		// 绑定Shader
		std::string proj_res_path(PROJ_RES_PATH);
		mp_shader = std::make_unique<Shader>(
			std::string(proj_res_path + "/Shaders/TestTexture2D.vert"),
			std::string(proj_res_path + "/Shaders/TestTexture2D.frag")
		);
		mp_shader->bind();
		mp_shader->setUniform1i("u_Texture", 1);
		mp_shader->unbind();
	}

	TestTexture2D::~TestTexture2D()
	{
		// 关闭混合
		GLCall(glDisable(GL_BLEND));

		mp_shader.reset();
		mp_VAO.reset();
		mp_VBO.reset();
		mp_texture2D.reset();
		mp_IBO.reset();
	}

	void TestTexture2D::onUpdate(float deltaTime, unsigned keyboardInput)
	{
		// 更新2D材质
		if (mp_texture2D == nullptr)
		{
			mp_texture2D = std::make_unique<GLTexture>(m_texturePath);
		}
		else if (m_texturePath != mp_texture2D->getFilePath())
		{
			mp_texture2D.reset(new GLTexture(m_texturePath));
		}

		// 更新uniform变量
		glm::mat4 mvp = m_proj * m_view * glm::translate(glm::mat4(1.0f), m_translation);
		mp_shader->bind();
		mp_shader->setUniformMat4f("u_MVP", mvp);
		mp_shader->unbind();
	}

	void TestTexture2D::onRender()
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		{
			Renderer renderer(nullptr);
			mp_texture2D->bind(1);
			renderer.draw(*mp_VAO, *mp_IBO, *mp_shader);
		}
	}

	void TestTexture2D::onImGuiRender()
	{
		ImGui::SliderFloat3("Img Position", &m_translation.x, -400.0f, 400.0f);

		if (ImGui::Button("Open File"))
		{
			IGFD::FileDialogConfig config;
			config.path = std::string(PROJ_RES_PATH);
			ImGuiFileDialog::Instance()->OpenDialog("ChooseTexture2D_Key", "Select a 2D texture", ".*", config);
		}
		if (ImGuiFileDialog::Instance()->Display("ChooseTexture2D_Key")) {
			if (ImGuiFileDialog::Instance()->IsOk()) { // action if OK
				m_texturePath = ImGuiFileDialog::Instance()->GetFilePathName();
				// std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
				// action
			}

			// close
			ImGuiFileDialog::Instance()->Close();
		}

		ImGui::Text("Avg %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}