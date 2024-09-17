#pragma once

#include "Test/base/Test.h"
#include "Core/Camera.h"
#include "Core/Object.h"
#include "OpenGL/glFrameBuffer.h"
#include "Render/Feature/Skybox.h"
#include "Render/Light/Light.h"

namespace test
{
	class TestSSAO : public Test
	{
	public:
		TestSSAO();
		~TestSSAO() override;

		void onUpdate(float deltaTime, const Input& hardwareInput) override;
		void onRender() override;
		void onImGuiRender() override;

	protected:
		void processInput(const Input& hardware_input, float deltaTime) const;

	protected:
		glm::mat4 m_proj = glm::mat4(1.0f);
		glm::mat4 m_view = glm::mat4(1.0f);

		std::unique_ptr<GLCore::Camera> m_pCamera;

		std::unique_ptr<GLCore::GLFrameBuffer> m_pGBufferFBO;
		std::unique_ptr<GLCore::GLShader> m_pGBufferShader;

		std::unique_ptr<GLCore::GLObject> m_pScrQuad;
		// std::unique_ptr<GLCore::GLShader> m_pScrQuadShader;

		std::vector<std::unique_ptr<GLCore::GLObject>> m_pObjects;
		std::vector<std::unique_ptr<GLCore::Light>> m_pLights;
	};
}

