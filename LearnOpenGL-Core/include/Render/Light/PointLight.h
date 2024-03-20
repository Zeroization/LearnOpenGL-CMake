#pragma once
#include "Render/Light/Light.h"

namespace GLCore
{
	class PointLight : public Light
	{
	public:
		PointLight(glm::vec3 color = glm::vec3(1.0f), glm::vec3 position = glm::vec3(0.0f, 0.0f, 1.0f),
				   float linear = 0.09f, float quadratic = 0.032f);

		void onImGuiRender(const std::string& ObjectName) override;

		inline float getLinear() const { return m_linear; }
		inline float getQuadratic() const { return m_quadratic; }

	private:
		float m_linear;
		float m_quadratic;
	};
}
