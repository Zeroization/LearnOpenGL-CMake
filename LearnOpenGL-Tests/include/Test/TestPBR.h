#pragma once

#include "Test/base/Test.h"
#include "Core/Camera.h"
#include "Core/Object.h"
#include "OpenGL/glFrameBuffer.h"
#include "Render/Feature/Skybox.h"
#include "Render/Light/Light.h"

namespace test
{
	class TestPBR : public Test
	{
	public:
		TestPBR();
		~TestPBR() override;

		void onUpdate(float deltaTime, const Input& hardwareInput) override;
		void onRender() override;
		void onImGuiRender() override;

	protected:
		void processInput(const Input& hardware_input, float deltaTime) const;

	protected:
		glm::mat4 m_proj = glm::mat4(1.0f);
		glm::mat4 m_view = glm::mat4(1.0f);

		std::unique_ptr<GLCore::Camera> m_pCamera;
		std::vector<std::unique_ptr<GLCore::GLObject>> m_pObjects;
		std::vector<std::shared_ptr<GLCore::GLTexture>> m_pTextures;
		std::vector<std::unique_ptr<GLCore::Light>> m_pLights;

		bool m_enableTexture = false;
		int m_nrRows = 7;
		int m_nrColumns = 7;
		float m_spacing = 2.5;
	};
}
