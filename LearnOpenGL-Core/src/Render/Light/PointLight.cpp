#include "Render/Light/PointLight.h"

namespace GLCore
{
    PointLight::PointLight(glm::vec3 color, glm::vec3 position, float linear, float quadratic)
        : Light(color, position, LightType::PointLight)
    {
        m_linear = linear;
        m_quadratic = quadratic;
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
}
