#pragma once

#include "pch.hpp"
#include "Core/Camera.h"

#include "Test/base/Test.h"
#include "Core/Object.h"

namespace test
{
	enum class ImgBlendMode
	{
		Normal = 0,		// 正常
		Dissolve,		// 溶解

		Darken,			// 变暗
		Multiply,		// 正片叠底
		ColorBurn,		// 颜色加深
		LinearBurn,		// 线性加深
		DarkerColor,	// 深色

		Lighten,		// 变亮
		Screen,			// 滤色
		ColorDodge,		// 颜色减淡
		LinearDodge,	// 线性减淡/添加
		LighterColor,	// 浅色

		Overlay,		// 叠加
		SoftLight,		// 柔光
		HardLight,		// 强光
		VividLight,		// 亮光
		LinearLight,	// 线性光
		PinLight,		// 点光
		HardMix,		// 实色混合

		Difference,		// 差值
		Exclusion,		// 排除

		Subtract,		// 减去
		Divide,			// 划分
	};

	class TestImgBlend : public Test
	{
	public:
		TestImgBlend();
		~TestImgBlend() override;

		void onUpdate(float deltaTime, const Input& hardwareInput) override;
		void onRender() override;
		void onImGuiRender() override;

	protected:
		void processInput(const Input& hardware_input, float deltaTime) const;

	private:
		std::unique_ptr<GLCore::GLObject> m_pTex2D;
		std::unique_ptr<GLCore::Camera> m_pCamera;
		std::unique_ptr<GLCore::GLTexture> m_pBaseImgLayer;
		std::unique_ptr<GLCore::GLTexture> m_pBlendImgLayer;

		glm::vec3 m_blendColorLayer;
		bool m_useImageBlend = false;
		float m_blendOpacity = 0.5f;
		ImgBlendMode m_blendMode = ImgBlendMode::Normal;
	};
}