#pragma once

#include "pch.hpp"

#include "Test/base/Test.h"
#include "Core/Object.h"

namespace test
{
	class TestTexture2D : public Test
	{
	public:
		TestTexture2D();
		~TestTexture2D() override;

		void onUpdate(float deltaTime, const Input& hardwareInput) override;
		void onRender() override;
		void onImGuiRender() override;

	private:

		std::unique_ptr<GLCore::GLObject> mp_tex2D;

		std::string m_texturePath;
		std::string m_lastTexturePath;

		glm::vec3 m_translation;
		glm::mat4 m_proj, m_view;
	};
}
