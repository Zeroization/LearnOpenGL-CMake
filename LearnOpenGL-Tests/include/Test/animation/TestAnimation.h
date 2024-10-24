#pragma once
#include "Test/base/Test.h"

#include "Core/Camera.h"
#include "Core/Object.h"
#include "Render/Feature/Skybox.h"
#include "Render/Light/Light.h"
#include "Animation/Animator.h"

namespace test
{
	class TestAnimation : public Test
	{
	public:
		TestAnimation();
		~TestAnimation() override;

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

		//TODO: 只是为了测试, 以后删掉
		std::unique_ptr<GLCore::Animation> m_pDanceAnimation;
		std::unique_ptr<GLCore::Animator> m_pAnimator;

		glm::mat4 m_proj = glm::mat4(1.0f);
		glm::mat4 m_view = glm::mat4(1.0f);

		bool m_selfSkyboxRender = true;
	};
}