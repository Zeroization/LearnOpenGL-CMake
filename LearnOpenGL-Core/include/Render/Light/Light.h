#pragma once

#include "Core/Object.h"
#include <vector>

namespace GLCore
{
	enum class LightType
	{
		DirectionalLight = 0,
		PointLight,
		SpotLight
	};

	constexpr int MAX_LIGHTS = 5;

	class Light : public GLObject
	{
	public:
		Light(glm::vec3 color = glm::vec3(1.0f), glm::vec3 position = glm::vec3(0.0f),
			  LightType lightType = LightType::PointLight);
		virtual ~Light() override;

		virtual void onImGuiRender(const std::string& ObjectName) override;
		virtual void updateUniforms(const std::vector<GLObject*>& objects) {}
		virtual void updateUniforms(const std::vector<std::unique_ptr<GLObject>>& objects);
		virtual void releaseUniforms(const std::vector<GLObject*>& objects) {}
		virtual void releaseUniforms(const std::vector<std::unique_ptr<GLObject>>& objects);

		inline LightType getLightType() const { return m_lightType; }
		inline std::string getLightTypeString() const
		{
			switch (m_lightType)
			{
				case LightType::DirectionalLight:	return "DirLight";
				case LightType::PointLight:			return "PointLight";
				case LightType::SpotLight:			return "SpotLight";
			}
			LOG_CRITICAL("[Light] return Undefined LightType!");
			__debugbreak();
			return {};
		}

	protected:
		LightType m_lightType;
	};
}