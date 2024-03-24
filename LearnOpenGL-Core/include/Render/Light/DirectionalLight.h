#pragma once

#include "Render/Light/Light.h"
#include <unordered_map>

namespace GLCore
{
	class DirectionalLight : public Light
	{
	public:
		DirectionalLight(glm::vec3 color = glm::vec3(1.0f), glm::vec3 position = glm::vec3(0.0f, 0.0f, 1.0f));
		virtual ~DirectionalLight() override;

		void onRender(const Renderer& renderer) override;

		void updateUniforms(const std::vector<GLObject*>& objects) override;
		void releaseUniforms(const std::vector<GLObject*>& objects) override;

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
	};
}

