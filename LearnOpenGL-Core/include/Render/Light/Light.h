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

	protected:
		LightType m_lightType;
	};
}