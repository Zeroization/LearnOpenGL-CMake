#include "Test/base/TestMenu.h"
#include "imgui/imgui.h"

namespace test
{
	TestMenu::TestMenu(Test*& pCurrentTest, GLFWwindow* window)
		: m_window(window), m_currentTest(pCurrentTest)
	{
	}

	TestMenu::~TestMenu()
	{
	}

	void TestMenu::onUpdate(float deltaTime, unsigned keyboardInput)
	{
	}

	void TestMenu::onImGuiRender()
	{
		for (auto& [name, func] : m_tests)
		{
			if (ImGui::Button(name.c_str()))
			{
				m_currentTest = func();
			}
		}
	}

	void TestMenu::onRender()
	{
	}
}
