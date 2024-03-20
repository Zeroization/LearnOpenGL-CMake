#include "Render/Light/DirectionalLight.h"

namespace GLCore
{
	DirectionalLight::DirectionalLight(glm::vec3 color, glm::vec3 position)
		: Light(color, position, LightType::DirectionalLight)
	{
		m_direction = glm::vec3(0.0f) - position;
	}
}
