#pragma once

#include "pch.hpp"
#include "Shader.h"
#include "Test/Test.h"

#include "OpenGL/glTexture.h"
#include "OpenGL/glVertexArray.h"
#include "OpenGL/glIndexBuffer.h"
#include "OpenGL/glVertexBuffer.h"

#include "imgui/thirdParty/FileBrowser/ImGuiFileDialog.h"
#include "imgui/thirdParty/FileBrowser/ImGuiFileDialogConfig.h"

namespace test
{
	class TestTexture2D : public Test
	{
	public:
		TestTexture2D();
		~TestTexture2D() override;

		void onUpdate(float deltaTime) override;
		void onRender() override;
		void onImGuiRender() override;

	private:
		std::unique_ptr<GLVertexArray> mp_VAO;
		std::unique_ptr<GLIndexBuffer> mp_IBO;
		std::unique_ptr<GLVertexBuffer> mp_VBO;
		std::unique_ptr<GLTexture> mp_texture2D;
		std::unique_ptr<Shader> mp_shader;

		std::string m_texturePath;

		glm::vec3 m_translation;
		glm::mat4 m_proj, m_view;
	};
}
