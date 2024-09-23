#pragma once

#include "pch.hpp"
#include "Core/Camera.h"

#include "Test/base/Test.h"
#include "Core/Object.h"

namespace test
{
	enum class ImgBlendMode
	{
		Normal = 0,		// ����
		Dissolve,		// �ܽ�

		Darken,			// �䰵
		Multiply,		// ��Ƭ����
		ColorBurn,		// ��ɫ����
		LinearBurn,		// ���Լ���
		DarkerColor,	// ��ɫ

		Lighten,		// ����
		Screen,			// ��ɫ
		ColorDodge,		// ��ɫ����
		LinearDodge,	// ���Լ���/���
		LighterColor,	// ǳɫ

		Overlay,		// ����
		SoftLight,		// ���
		HardLight,		// ǿ��
		VividLight,		// ����
		LinearLight,	// ���Թ�
		PinLight,		// ���
		HardMix,		// ʵɫ���

		Difference,		// ��ֵ
		Exclusion,		// �ų�

		Subtract,		// ��ȥ
		Divide,			// ����
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