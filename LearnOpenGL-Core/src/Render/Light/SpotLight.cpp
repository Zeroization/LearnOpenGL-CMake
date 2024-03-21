#include "Render/Light/SpotLight.h"

namespace GLCore
{
	SpotLight::SpotLight(glm::vec3 color, glm::vec3 position, glm::vec3 direction, float cutOff, float outerCutOff,
		float linear, float quadratic)
			: Light(color, position, LightType::SpotLight)
	{
		m_direction = direction;
		m_cutOff = cutOff;
		m_outerCutOff = outerCutOff;
		m_linear = linear;
		m_quadratic = quadratic;

		for (int i = 0; i <= m_sInstances.size(); ++i)
		{
			if (m_sInstances[i] == 0)
			{
				m_slot = i;
				m_sInstances[i] = 1;
				break;
			}
		}
	}

	SpotLight::~SpotLight()
	{
		m_sInstances.erase(m_slot);
	}

	void SpotLight::updateUniforms(const std::vector<GLObject*>& objects)
	{
		// 光源超出限制, 先补齐前面的空位(0~MAX)
		if (m_sInstances.size() >= MAX_LIGHTS)
		{
			bool isMoved = false;
			for (int i = 0; i < MAX_LIGHTS; ++i)
			{
				if (m_sInstances[i] == 0)
				{
					isMoved = true;
					m_sInstances.erase(m_slot);
					m_slot = i;
					m_sInstances[i] = 1;
					break;
				}
			}

			// 无法补齐时发出警告(仅创建光源时一次), 例如光源5会代替光源0
			static size_t lastSize = 0;
			if (!isMoved && lastSize != m_sInstances.size() && m_sInstances.size() > MAX_LIGHTS)
			{
				lastSize = m_sInstances.size();
				LOG_WARN(std::format("[Light] Directional Lights are max({}/{}), {} lights will be ignored...", m_sInstances.size(), MAX_LIGHTS, m_sInstances.size() - MAX_LIGHTS));
			}

		}
		// LOG_DEBUG(std::format("[Light] Directional Lights: setUniform at slot {}", m_slot % MAX_LIGHTS));
		for (auto& object : objects)
		{
			// 开始设置一个物体, 多光源的shader
			if (object)
			{
				object->setUniform(std::format("u_SpotLight[{}].position", m_slot % MAX_LIGHTS), m_translation);
				object->setUniform(std::format("u_SpotLight[{}].direction", m_slot % MAX_LIGHTS), getDirection());
				object->setUniform(std::format("u_SpotLight[{}].ambient", m_slot % MAX_LIGHTS), m_basicMaterial->ambient);
				object->setUniform(std::format("u_SpotLight[{}].diffuse", m_slot % MAX_LIGHTS), m_basicMaterial->diffuse);
				object->setUniform(std::format("u_SpotLight[{}].specular", m_slot % MAX_LIGHTS), m_basicMaterial->specular);
				object->setUniform(std::format("u_SpotLight[{}].cutOff", m_slot % MAX_LIGHTS), getCutOffCos());
				object->setUniform(std::format("u_SpotLight[{}].outerCutOff", m_slot % MAX_LIGHTS), getOuterCutOffCos());
				object->setUniform(std::format("u_SpotLight[{}].linear", m_slot % MAX_LIGHTS), m_linear);
				object->setUniform(std::format("u_SpotLight[{}].quadratic", m_slot % MAX_LIGHTS), m_quadratic);

			}
		}
	}

	void SpotLight::releaseUniforms(const std::vector<GLObject*>& objects)
	{
		for (auto& object : objects)
		{
			// 开始设置一个物体, 多光源的shader
			if (object)
			{
				object->setUniform(std::format("u_SpotLight[{}].position", m_slot % MAX_LIGHTS), glm::vec3(0.0f));
				object->setUniform(std::format("u_SpotLight[{}].direction", m_slot % MAX_LIGHTS), glm::vec3(0.0f));
				object->setUniform(std::format("u_SpotLight[{}].ambient", m_slot % MAX_LIGHTS), glm::vec3(0.0f));
				object->setUniform(std::format("u_SpotLight[{}].diffuse", m_slot % MAX_LIGHTS), glm::vec3(0.0f));
				object->setUniform(std::format("u_SpotLight[{}].specular", m_slot % MAX_LIGHTS), glm::vec3(0.0f));
				object->setUniform(std::format("u_SpotLight[{}].cutOff", m_slot % MAX_LIGHTS), 0.0f);
				object->setUniform(std::format("u_SpotLight[{}].outerCutOff", m_slot % MAX_LIGHTS), 0.0f);
				object->setUniform(std::format("u_SpotLight[{}].linear", m_slot % MAX_LIGHTS), 0.0f);
				object->setUniform(std::format("u_SpotLight[{}].quadratic", m_slot % MAX_LIGHTS), 0.0f);
			}
		}
	}
}
