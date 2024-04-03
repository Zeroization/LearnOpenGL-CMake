#pragma once

#include "OpenGL/glFrameBuffer.h"
#include "Test/TestMultipleLights.h"

namespace test
{
	class TestShadowMap : public TestMultipleLights
	{
	public:
		TestShadowMap();

		void onUpdate(float deltaTime, const Input& hardwareInput) override;
		void onRender() override;
		void onImGuiRender() override;

	protected:
		std::vector<std::unique_ptr<GLCore::GLFrameBuffer>> m_shadowMapFBOs;
		std::unique_ptr<GLCore::GLShader> m_pShadowShader;


		int m_testWindowWidth, m_testWindowHeight;
	};

}
