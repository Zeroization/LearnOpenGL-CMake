#pragma once

#include "Render/Light/Light.h"

namespace GLCore
{
	class SpotLight : public Light
	{
	public:
		SpotLight(glm::vec3 color = glm::vec3(1.0f), glm::vec3 position = glm::vec3(0.0f, 0.0f, 1.0f),
				   float cutOff = 12.5f, float outerCutOff = 17.5f,
				   float linear = 0.014f, float quadratic = 0.0007f);
		virtual ~SpotLight() override;

		void onRender(const Renderer& renderer) override;

		void updateUniforms(const std::vector<GLObject*>& objects) override;
		void releaseUniforms(const std::vector<GLObject*>& objects) override;

		inline float getCutOffCos() const { return glm::cos(glm::radians(m_cutOff)); }
		inline float getOuterCutOffCos() const { return glm::cos(glm::radians(m_outerCutOff)); }
		inline float getLinear() const { return m_linear; }
		inline float getQuadratic() const { return m_quadratic; }
		inline GLObject* getArrow() const { return m_pArrow.get(); }
		inline const glm::vec3& getDirection()
		{
			glm::quat rot(glm::radians(m_pArrow->getRotation()));
			glm::quat direction = rot * m_sOriginDirection * glm::conjugate(rot);
			m_direction = glm::vec3(direction.x, direction.y, direction.z);

			return m_direction;
		}

	private:
		inline static std::unordered_map<int, int> m_sInstances;
		inline static glm::quat m_sOriginDirection;
		int m_slot;

		glm::vec3 m_direction;
		std::unique_ptr<GLObject> m_pArrow;
		float m_cutOff, m_outerCutOff;
		float m_linear, m_quadratic;
	};
}
