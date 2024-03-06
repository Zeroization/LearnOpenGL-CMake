#pragma once

#include "Test/base/Test.h"

namespace test
{
	class TestClearColor : public Test
	{
	public:
		TestClearColor();
		~TestClearColor() override;

		void onUpdate(float deltaTime, unsigned keyboardInput) override;
		void onRender() override;
		void onImGuiRender() override;
	private:
		glm::vec4 m_color;
	};
}
