#pragma once
#include "Test/base/Test.h"

#include "Core/Camera.h"
#include "Core/Object.h"
#include "Render/Feature/Skybox.h"

#include "Render/Light/Light.h"

namespace test
{
	class TestMultipleLights : public Test
	{
	public:
		TestMultipleLights();
		~TestMultipleLights() override;

		void onUpdate(float deltaTime, const Input& hardwareInput) override;
		void onRender() override;
		void renderSkybox(const GLCore::Renderer& renderer) const;
		void onImGuiRender() override;

	private:
		void processInput(const Input& hardware_input, float deltaTime) const;

	protected:
		std::unique_ptr<GLCore::Camera> m_pCamera;
		std::unique_ptr<GLCore::SkyBox> m_pSkybox;
		std::vector<std::unique_ptr<GLCore::GLObject>> m_pObjects;
		std::vector<std::unique_ptr<GLCore::Light>> m_pLights;

		glm::mat4 m_proj = glm::mat4(1.0f);
		glm::mat4 m_view = glm::mat4(1.0f);

		bool m_selfSkyboxRender = true;
	};
}