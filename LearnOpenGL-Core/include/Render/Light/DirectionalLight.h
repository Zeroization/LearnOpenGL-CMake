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

		void updateUniforms(const std::vector<GLObject*>& objects) override;
		void releaseUniforms(const std::vector<GLObject*>& objects) override;

		inline const glm::vec3& getDirection() { m_direction = glm::vec3(0.0f) - m_translation; return m_direction; }

	private:
		inline static std::unordered_map<int, int> m_sInstances;
		int m_slot;

		//todo 可能会修改为position和direction分开用
		glm::vec3 m_direction;
	};
}

