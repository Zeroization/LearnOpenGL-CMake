#pragma once

#include "Render/Light/Light.h"

namespace GLCore
{
	class DirectionalLight : public Light
	{
	public:
		DirectionalLight(glm::vec3 color = glm::vec3(1.0f), glm::vec3 position = glm::vec3(0.0f, 0.0f, 1.0f));

		inline const glm::vec3& getDirection() { m_direction = glm::vec3(0.0f) - m_translation; return m_direction; }

	private:
		glm::vec3 m_direction;
	};
}

