#pragma once

#include "Test/base/Test.h"

#include "Core/Camera.h"
#include "Core/Object.h"
#include "Render/Light/Light.h"

namespace test
{
	class TestBlinnPhong : public Test
	{
	public:
		TestBlinnPhong();
		~TestBlinnPhong() override;

		void onUpdate(float deltaTime, const Input& hardwareInput) override;
		void onRender() override;
		void onImGuiRender() override;

	private:
		void processInput(const Input& hardware_input, float deltaTime) const;

	private:
		std::unique_ptr<GLCore::Camera> m_pCamera;
		std::unique_ptr<GLCore::GLObject> m_pWoodBox;
		std::unique_ptr<GLCore::Light> m_pLight;

		glm::mat4 m_proj = glm::mat4(1.0f);
		glm::mat4 m_view = glm::mat4(1.0f);

		bool m_enableAmbientLight;
		bool m_enableDiffuseLight;
		bool m_enableSpecularLight;
	};
}
