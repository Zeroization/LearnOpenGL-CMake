#include "Test/Test.h"
#include "imgui/imgui.h"

namespace test
{
	TestMenu::TestMenu(Test*& currentTest)
		: m_currentTest(currentTest)
	{

	}

	TestMenu::~TestMenu()
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
}
