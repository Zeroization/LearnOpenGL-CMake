#pragma once

#include "OpenGL/glFrameBuffer.h"
#include "Test/TestMultipleLights.h"

namespace test
{
	class TestEnvMapping : public TestMultipleLights
	{
	public:
		TestEnvMapping();

		void onUpdate(float deltaTime, const Input& hardwareInput) override;
		void onRender() override;
		void onImGuiRender() override;

	protected:
		int m_envMappingParam;
		float m_refractionRatio;
		std::vector<std::unique_ptr<GLCore::GLObject>> m_pEnvMapObjects;
		std::unique_ptr<GLCore::GLCubeMapTexture> m_pDynamicEnvMap;
		std::unique_ptr<GLCore::GLFrameBuffer> m_pDynEnvMapFBO;

		int m_testWindowWidth, m_testWindowHeight;
	};
}