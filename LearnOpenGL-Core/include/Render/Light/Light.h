#pragma once

#include "pch.hpp"
#include "Core/Object.h"

namespace GLCore
{
	enum class LightType
	{
		DirectionalLight = 0,
		PointLight,
		SpotLight
	};

	class Light : public GLObject
	{
	public:
		Light(glm::vec3 color = glm::vec3(1.0f), glm::vec3 position = glm::vec3(0.0f),
			  LightType lightType = LightType::PointLight);
		virtual ~Light() override;

		virtual void onImGuiRender(const std::string& ObjectName) override;

		inline LightType getLightType() const { return m_lightType; }
		inline std::string getLightTypeString() const
		{
			switch (m_lightType)
			{
				case LightType::DirectionalLight:	return "DirLight";
				case LightType::PointLight:			return "PointLight";
				case LightType::SpotLight:			return "SpotLight";
			}
		}

	protected:
		LightType m_lightType;
	};
}