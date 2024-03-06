#pragma once

namespace test
{
	class Test
	{
	public:
		Test() = default;
		virtual ~Test() = default;

		virtual void onUpdate(float deltaTime, unsigned keyboardInput) = 0;
		virtual void onRender() = 0;
		virtual void onImGuiRender() = 0;
	};
}
