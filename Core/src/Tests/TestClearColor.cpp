#include "Tests\TestClearColor.h"

#include "pch.hpp"
#include "OpenGL/glDebug.h"
#include "imgui/imgui.h"

test::TestClearColor::TestClearColor()
	: m_color(0.2f, 0.3f, 0.8f, 1.0f)
{
}

test::TestClearColor::~TestClearColor()
{
}

void test::TestClearColor::onUpdate(float deltaTime)
{
}

void test::TestClearColor::onRender()
{
	GLCall(glClearColor(m_color.r, m_color.g, m_color.b, m_color.a));
	GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void test::TestClearColor::onImGuiRender()
{
	ImGui::ColorEdit4("Clear Color", &m_color.r);
}
