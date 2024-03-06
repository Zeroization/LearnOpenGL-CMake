#include "Test\TestClearColor.h"

#include "pch.hpp"
#include "OpenGL/glDebug.h"
#include "imgui/imgui.h"

namespace test
{
	TestClearColor::TestClearColor()
		: m_color(0.2f, 0.3f, 0.8f, 1.0f)
	{
	}

	TestClearColor::~TestClearColor()
	{
	}

	void TestClearColor::onUpdate(float deltaTime, unsigned keyboardInput)
	{
	}

	void TestClearColor::onRender()
	{
		// LOG_DEBUG("{0} {1}", "[Test::ClearColor]", "TestClearColor::onRenderer");
		GLCall(glClearColor(m_color.r, m_color.g, m_color.b, m_color.a));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));
	}

	void TestClearColor::onImGuiRender()
	{
		// LOG_DEBUG("{0} {1}", "[Test::ClearColor]", "TestClearColor::onImGuiRender");
		ImGui::ColorEdit4("ClearColor Test", &m_color.r);
	}
}