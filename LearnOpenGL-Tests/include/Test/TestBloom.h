#pragma once

#include "TestMultipleLights.h"
#include "OpenGL/glFrameBuffer.h"

namespace test
{
	class TestBloom : public TestMultipleLights
	{
	public:
		TestBloom();
		~TestBloom();

		void onUpdate(float deltaTime, const Input& hardwareInput) override;
		void onRender() override;
		void onImGuiRender() override;

	private:
		void renderHDRQuad(int step) const;

	private:
		std::unique_ptr<GLCore::GLFrameBuffer> m_pFBO;

		std::unique_ptr<GLCore::GLObject> m_pScreenQuad;
		std::unique_ptr<GLCore::GLShader> m_pScreenQuadShader;
		std::unique_ptr<GLCore::GLShader> m_pBlurShader;
		std::vector<std::unique_ptr<GLCore::GLFrameBuffer>> m_pBlurFBOs;

		bool m_enableHDR;
		float m_exposure = 0.1;
	};
}
