#pragma once
#include "Render/Light/Light.h"
#include <unordered_map>

namespace GLCore
{
	class PointLight : public Light
	{
	public:
		PointLight(glm::vec3 color = glm::vec3(1.0f), glm::vec3 position = glm::vec3(0.0f, 0.0f, 1.0f),
				   float linear = 0.09f, float quadratic = 0.032f);
		virtual ~PointLight() override;

		void onImGuiRender(const std::string& ObjectName) override;
		void updateUniforms(const std::vector<GLObject*>& objects) override;
		void releaseUniforms(const std::vector<GLObject*>& objects) override;

		inline float getLinear() const { return m_linear; }
		inline float getQuadratic() const { return m_quadratic; }
		inline float getVolumeRadius()
		{
			if (m_radius >= 0 && 
				std::fabs(m_lastLinear - m_linear) < 0.001 && 
				std::fabs(m_lastQuadratic - m_quadratic) < 0.001)
			{
				return m_radius;
			}

			m_lastLinear = m_linear;
			m_lastQuadratic = m_quadratic;

			glm::vec3 lightColor = m_basicMaterial->diffuse;
			float lightMax = std::fmaxf(std::fmaxf(lightColor.r, lightColor.g), lightColor.b);
			m_radius = (-m_linear + std::sqrtf(m_linear * m_linear - 4 * m_quadratic * (1.0 - (256.0 / 5.0) * lightMax))) / (2 * m_quadratic);

			return m_radius;
		}

	private:
		inline static std::unordered_map<int, int> m_sInstances;
		int m_slot;

		float m_linear, m_lastLinear;
		float m_quadratic, m_lastQuadratic;
		float m_radius = -5;
	};
}
