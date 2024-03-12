#pragma once

#include "pch.hpp"

#include "Test/base/Test.h"

#include "OpenGL/glTexture.h"
#include "OpenGL/glShader.h"
#include "OpenGL/glVertexArray.h"
#include "OpenGL/glIndexBuffer.h"
#include "OpenGL/glVertexBuffer.h"

namespace test
{
	class TestTexture2D : public Test
	{
	public:
		TestTexture2D();
		~TestTexture2D() override;

		void onUpdate(float deltaTime, const Input& hardwareInput) override;
		void onRender() override;
		void onImGuiRender() override;

	private:
		std::unique_ptr<GLVertexArray> mp_VAO;
		std::unique_ptr<GLIndexBuffer> mp_IBO;
		std::unique_ptr<GLVertexBuffer> mp_VBO;
		std::unique_ptr<GLTexture> mp_texture2D;
		std::unique_ptr<GLShader> mp_shader;

		std::string m_texturePath;

		glm::vec3 m_translation;
		glm::mat4 m_proj, m_view;
	};
}
