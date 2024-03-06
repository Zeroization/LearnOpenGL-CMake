#pragma once

#include "Test/base/Test.h"
#include <functional>

namespace test
{
	class TestMenu : public Test
	{
	public:
		TestMenu(Test*& pCurrentTest, GLFWwindow* window = nullptr);
		virtual ~TestMenu() override;

		void onUpdate(float deltaTime, unsigned keyboardInput) override;
		void onImGuiRender() override;
		void onRender() override;

		template<typename T>
		void registerTest(const std::string& name)
		{
			LOG_INFO("[{0}] {1}", "Test", std::format("Registering test: {}...", name));
			m_tests.push_back(std::make_pair(name, []() { return new T(); }));
		}

	private:
		GLFWwindow* m_window;
		Test*& m_currentTest;
		std::vector<std::pair<std::string, std::function<Test*()>>> m_tests;
	};
}
