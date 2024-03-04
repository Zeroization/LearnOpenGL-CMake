#include "Test\TestTexture2D.h"

#include "pch.hpp"
#include "OpenGL/glDebug.h"
#include "imgui/imgui.h"

test::TestTexture2D::TestTexture2D()
{
}

test::TestTexture2D::~TestTexture2D()
{
}

void test::TestTexture2D::onUpdate(float deltaTime)
{
}

void test::TestTexture2D::onRender()
{
	GLCall(glClear(GL_COLOR_BUFFER_BIT));

}

void test::TestTexture2D::onImGuiRender()
{
}
