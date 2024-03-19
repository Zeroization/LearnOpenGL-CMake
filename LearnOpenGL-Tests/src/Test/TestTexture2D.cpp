#include "Test/TestTexture2D.h"

#include "pch.hpp"
#include "Core/Renderer.h"

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

		const std::string proj_res_path(PROJ_RES_PATH);
		mp_tex2D = std::make_unique<GLCore::GLObject>(positions, sizeof(positions),
		                                              GLCore::GLVertexBufferLayout({2, 2}),
		                                              indices, 6,
		                                              std::string(proj_res_path + "/Shaders/TestTexture2D/shader.vert"),
		                                              std::string(proj_res_path + "/Shaders/TestTexture2D/shader.frag"));
	}

	TestTexture2D::~TestTexture2D()
	{
		// 关闭混合
		GLCall(glDisable(GL_BLEND));

		mp_tex2D.reset();
	}

	void TestTexture2D::onUpdate(float deltaTime, const Input& hardwareInput)
	{
		// 更新2D材质
		if (m_texturePath != m_lastTexturePath && m_texturePath.length() >= 1)
		{
			m_lastTexturePath= m_texturePath;
			mp_tex2D->resetTextures({{m_texturePath}});
		}

		// 更新uniform变量
		glm::mat4 mvp = m_proj * m_view * glm::translate(glm::mat4(1.0f), m_translation);
		mp_tex2D->setUniform("u_MVP", mvp);
	}

	void TestTexture2D::onRender()
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		{
			GLCore::Renderer renderer(nullptr);
			mp_tex2D->onRender(renderer);
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
			}

			// close
			ImGuiFileDialog::Instance()->Close();
		}

		ImGui::Text("Avg %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}