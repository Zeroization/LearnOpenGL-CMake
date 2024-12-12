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

		std::vector<std::unique_ptr<GLCore::GLObject>> m_pCubesForIK;
		std::unique_ptr<GLCore::GLObject> m_pTargetForIk;
		glm::vec3 m_ikTargetPos = glm::vec3(0.0f);

		glm::mat4 m_proj = glm::mat4(1.0f);
		glm::mat4 m_view = glm::mat4(1.0f);

		bool m_selfSkyboxRender = true;

		// 动画相关
		bool m_isEnableAnimation = false;
		// 动画相关 - 混合
		bool m_isEnableLerpBlending = false;
		bool m_isEnableCrossFadeBlending = false;
		bool m_isEnablePartialBlending = false;
		bool m_isEnableAdditiveBlending = false;
		// 动画相关 - IK
		bool m_isEnableIK = false;
		int m_ikCurOpt = 0;			// 0 - None; 1 - two-bone; 2 - CCD; 3 - FABRIK
		int m_ikBonesCnt = 4;
		int m_ikIterationCnt = 2;
		GLCore::IkChainParams m_ikChainParam;
	};
}