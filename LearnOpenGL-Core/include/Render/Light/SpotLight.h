#pragma once

#include "Render/Light/Light.h"

namespace GLCore
{
	class SpotLight : public Light
	{
	public:
		SpotLight(glm::vec3 color = glm::vec3(1.0f), glm::vec3 position = glm::vec3(0.0f, 0.0f, 1.0f),
				   glm::vec3 direction = glm::vec3(0.0f, 0.0f, 1.0f),
				   float cutOff = 12.5f, float outerCutOff = 17.5f,
				   float linear = 0.014f, float quadratic = 0.0007f);
		virtual ~SpotLight() override;

		void updateUniforms(const std::vector<GLObject*>& objects) override;
		void releaseUniforms(const std::vector<GLObject*>& objects) override;

		inline float getCutOffCos() const { return glm::cos(glm::radians(m_cutOff)); }
		inline float getOuterCutOffCos() const { return glm::cos(glm::radians(m_outerCutOff)); }
		inline const glm::vec3& getDirection() const { return m_direction; }
		inline float getLinear() const { return m_linear; }
		inline float getQuadratic() const { return m_quadratic; }

	private:
		inline static std::unordered_map<int, int> m_sInstances;
		int m_slot;

		// todo 可能会修改为position和direction分开用
		// todo 等能显示出坐标轴了一定要改！
		glm::vec3 m_direction;
		float m_cutOff, m_outerCutOff;
		float m_linear, m_quadratic;
	};
}
