#include "Render/Light/DirectionalLight.h"

namespace GLCore
{
	DirectionalLight::DirectionalLight(glm::vec3 color, glm::vec3 position)
		: Light(color, position, LightType::DirectionalLight)
	{
		m_direction = glm::vec3(0.0f) - position;
		m_sOriginDirection = glm::quat(0.0f, m_direction);

		for (int i = 0; i <= m_sInstances.size(); ++i)
		{
			if (m_sInstances[i] == 0)
			{
				m_slot = i;
				m_sInstances[i] = 1;
				break;
			}
		}

		m_pArrow = std::make_unique<GLObject>(std::string(PROJ_RES_PATH) + "/Models/Simple/Axis.obj",
											  std::string(PROJ_RES_PATH) + "/Shaders/CustomModel/model.vert",
											  std::string(PROJ_RES_PATH) + "/Shaders/CustomModel/Axis/axis.frag");

		m_pArrow->setColor(80.0f, 140.0f, 160.0f);
		m_pArrow->setScale(0.1f, 0.1f, 0.1f);
	}

	DirectionalLight::~DirectionalLight()
	{
		m_sInstances.erase(m_slot);
	}

	void DirectionalLight::onRender(const Renderer& renderer)
	{
		if (m_isVisible)
		{
			this->bind();
			renderer.draw(*m_modelData.pRaw->VAO, *m_modelData.pRaw->IBO, m_material->getShader());

			m_pArrow->onRender(renderer);
		}
	}

	void DirectionalLight::updateUniforms(const std::vector<GLObject*>& objects)
	{
		// ��Դ��������, �Ȳ���ǰ��Ŀ�λ(0~MAX)
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

			// �޷�����ʱ��������(��������Դʱһ��), �����Դ5������Դ0
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
			// ��ʼ����һ������, ���Դ��shader
			if (object)
			{
				// ���ñ���Shader
				object->setUniform(std::format("u_DirLight[{}].direction", m_slot % MAX_LIGHTS), getDirection());
				object->setUniform(std::format("u_DirLight[{}].ambient", m_slot % MAX_LIGHTS), m_basicMaterial->ambient);
				object->setUniform(std::format("u_DirLight[{}].diffuse", m_slot % MAX_LIGHTS), m_basicMaterial->diffuse);
				object->setUniform(std::format("u_DirLight[{}].specular", m_slot % MAX_LIGHTS), m_basicMaterial->specular);
			}
		}
	}

	void DirectionalLight::releaseUniforms(const std::vector<GLObject*>& objects)
	{
		for (auto& object : objects)
		{
			if (object)
			{
				object->setUniform(std::format("u_DirLight[{}].direction", m_slot % MAX_LIGHTS), glm::vec3(0.0f));
				object->setUniform(std::format("u_DirLight[{}].ambient", m_slot % MAX_LIGHTS), glm::vec3(0.0f));
				object->setUniform(std::format("u_DirLight[{}].diffuse", m_slot % MAX_LIGHTS), glm::vec3(0.0f));
				object->setUniform(std::format("u_DirLight[{}].specular", m_slot % MAX_LIGHTS), glm::vec3(0.0f));
			}
		}
	}
}
