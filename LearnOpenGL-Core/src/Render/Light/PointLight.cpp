#include "Render/Light/PointLight.h"

namespace GLCore
{
    PointLight::PointLight(glm::vec3 color, glm::vec3 position, float linear, float quadratic)
        : Light(color, position, LightType::PointLight)
    {
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

    PointLight::~PointLight()
    {
		m_sInstances.erase(m_slot);
    }

    void PointLight::onImGuiRender(const std::string& ObjectName)
    {
        const std::string objID("##" + std::to_string(m_uuid()));
        if (ImGui::CollapsingHeader((ObjectName + objID).c_str()))
        {
            ImGui::SeparatorText(std::string("Transforms" + objID).c_str());
            ImGui::DragFloat3(std::string("Rotation (Euler Angle)" + objID).c_str(), &m_rotation.x, 0.25f, -360.0f, 360.0f);
            ImGui::DragFloat3(std::string("Translation" + objID).c_str(), &m_translation.x, 0.25f, -100.0f, 100.0f);

            ImGui::SeparatorText(std::string("Material" + objID).c_str());
            ImGui::DragFloat3(std::string("Ambient" + objID).c_str(), &m_basicMaterial->ambient.r, 0.005f, 0.0f, 1.0f);
            ImGui::DragFloat3(std::string("Diffuse" + objID).c_str(), &m_basicMaterial->diffuse.r, 0.005f, 0.0f, 1.0f);
            ImGui::DragFloat3(std::string("Specular" + objID).c_str(), &m_basicMaterial->specular.r, 0.005f, 0.0f, 1.0f);

            ImGui::SeparatorText(std::string("Attributes" + objID).c_str());
            ImGui::Checkbox(std::string("isVisible" + objID).c_str(), &m_isVisible);
            ImGui::DragFloat(std::string("Linear" + objID).c_str(), &m_linear, 0.0005f, 0.0f, 1.0f);
            ImGui::DragFloat(std::string("Quadratic" + objID).c_str(), &m_quadratic, 0.0005f, 0.0f, 1.0f);
            ImGui::Text(std::format("ObjID: {}", std::to_string(m_uuid())).c_str());
        }
    }

    void PointLight::updateUniforms(const std::vector<GLObject*>& objects)
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
				object->setUniform(std::format("u_PointLight[{}].position", m_slot % MAX_LIGHTS), m_translation);
				object->setUniform(std::format("u_PointLight[{}].ambient", m_slot % MAX_LIGHTS), m_basicMaterial->ambient);
				object->setUniform(std::format("u_PointLight[{}].diffuse", m_slot % MAX_LIGHTS), m_basicMaterial->diffuse);
				object->setUniform(std::format("u_PointLight[{}].specular", m_slot % MAX_LIGHTS), m_basicMaterial->specular);
				object->setUniform(std::format("u_PointLight[{}].linear", m_slot % MAX_LIGHTS), m_linear);
				object->setUniform(std::format("u_PointLight[{}].quadratic", m_slot % MAX_LIGHTS), m_quadratic);
			}
		}
    }

    void PointLight::releaseUniforms(const std::vector<GLObject*>& objects)
    {
		for (auto& object : objects)
		{
			// 开始设置一个物体, 多光源的shader
			if (object)
			{
				object->setUniform(std::format("u_PointLight[{}].position", m_slot % MAX_LIGHTS), glm::vec3(0.0f));
				object->setUniform(std::format("u_PointLight[{}].ambient", m_slot % MAX_LIGHTS), glm::vec3(0.0f));
				object->setUniform(std::format("u_PointLight[{}].diffuse", m_slot % MAX_LIGHTS), glm::vec3(0.0f));
				object->setUniform(std::format("u_PointLight[{}].specular", m_slot % MAX_LIGHTS), glm::vec3(0.0f));
				object->setUniform(std::format("u_PointLight[{}].linear", m_slot % MAX_LIGHTS), 0.0f);
				object->setUniform(std::format("u_PointLight[{}].quadratic", m_slot % MAX_LIGHTS), 0.0f);
			}
		}
    }
}
