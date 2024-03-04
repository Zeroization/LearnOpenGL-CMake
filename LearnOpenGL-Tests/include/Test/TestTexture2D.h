#pragma once

#include "Test/Test.h"

namespace test
{
	class TestTexture2D : public Test
	{
	public:
		TestTexture2D();
		~TestTexture2D() override;

		void onUpdate(float deltaTime) override;
		void onRender() override;
		void onImGuiRender() override;
	};
}
