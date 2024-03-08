#pragma once

namespace test
{
	struct Input
	{
		unsigned keyboardInput = 0;

		double mouseMovXOffset = 0.0f;
		double mouseMovYOffset = 0.0f;
		double mouseScrollYOffset = 0.0f;

		inline void clear()
		{
			keyboardInput = 0;
			mouseMovXOffset = 0.0f;
			mouseMovYOffset = 0.0f;
			mouseScrollYOffset = 0.0f;
		}
	};

	class Test
	{
	public:
		Test() = default;
		virtual ~Test() = default;

		virtual void onUpdate(float deltaTime, const Input& hardwareInput) = 0;
		virtual void onRender() = 0;
		virtual void onImGuiRender() = 0;
	};
}
