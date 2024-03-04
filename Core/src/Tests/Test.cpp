#include "Tests/Test.h"
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
		for (auto& test : m_tests)
		{
			if (ImGui::Button(test.first.c_str()))
			{
				m_currentTest = test.second();
			}
		}
	}
}
