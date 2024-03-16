#pragma once

#include "Test/base/Test.h"

#include "OpenGL/glVertexArray.h"
#include "OpenGL/glIndexBuffer.h"
#include "OpenGL/glVertexBuffer.h"
#include "OpenGL/glShader.h"

#include "Core/Camera.h"
#include "Core/Object.h"


namespace test
{
	class TestCamera : public Test
	{
	public:
		TestCamera();
		~TestCamera() override;

		void onUpdate(float deltaTime, const Input& hardwareInput) override;
		void onRender() override;
		void onImGuiRender() override;

	private:
		void processInput(const Input& hardware_input, float deltaTime) const;

	private:
		std::unique_ptr<Camera> m_pCamera;
		std::unique_ptr<GLObject> m_pCube;

		glm::vec3 m_translation;
		glm::vec4 m_color;
		glm::mat4 m_proj = glm::mat4(1.0f), m_view = glm::mat4(1.0f);
	};
}
