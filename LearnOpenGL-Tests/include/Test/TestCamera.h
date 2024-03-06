#pragma once

#include "Test/base/Test.h"

#include "OpenGL/glVertexArray.h"
#include "OpenGL/glIndexBuffer.h"
#include "OpenGL/glVertexBuffer.h"

#include "Camera.h"
#include "Shader.h"

namespace test
{
	class TestCamera : public Test
	{
	public:
		TestCamera();
		~TestCamera() override;

		void onUpdate(float deltaTime, unsigned keyboardInput) override;
		void onRender() override;
		void onImGuiRender() override;

	private:
		void processInput(const unsigned keyboardInput, float deltaTime) const;

	private:
		std::unique_ptr<Camera> mp_camera;
		std::unique_ptr<GLVertexArray> mp_VAO;
		std::unique_ptr<GLIndexBuffer> mp_IBO;
		std::unique_ptr<GLVertexBuffer> mp_VBO;
		std::unique_ptr<Shader> mp_shader;

		glm::vec3 m_translation;
		glm::vec4 m_color;
		glm::mat4 m_proj = glm::mat4(1.0f), m_view = glm::mat4(1.0f);
	};
}
