#pragma once

#include "TestMultipleLights.h"
#include "OpenGL/glFrameBuffer.h"

namespace test
{
	class TestHDR : public TestMultipleLights
	{
	public:
		TestHDR();
		~TestHDR();

		void onUpdate(float deltaTime, const Input& hardwareInput) override;
		void onRender() override;
		void onImGuiRender() override;

	private:
		void renderHDRQuad();

	private:
		std::unique_ptr<GLCore::GLObject> m_pScreenQuad;
		std::unique_ptr<GLCore::GLFrameBuffer> m_pHDRFBO;
		bool m_enableHDR;
		float m_exposure = 0.1;
	};
}
