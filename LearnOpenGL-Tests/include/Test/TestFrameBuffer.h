#pragma once
#include "OpenGL/glFrameBuffer.h"
#include "Test/TestMultipleLights.h"

namespace test
{
	class TestFrameBuffer : public TestMultipleLights
	{
	public:
		TestFrameBuffer();

		void onUpdate(float deltaTime, const Input& hardwareInput) override;
		void onRender() override;
		void onImGuiRender() override;

	protected:
		std::unique_ptr<GLCore::GLObject> m_pScreenQuad;
		std::unique_ptr<GLCore::GLFrameBuffer> m_pFBO;
	};
}

