#include "Render/Light.h"

namespace GLCore
{
    static float lightCubeVertices[] =
    {
        -0.5f, -0.5f, -0.5f, // 0
         0.5f, -0.5f, -0.5f, // 1
         0.5f,  0.5f, -0.5f, // 2
        -0.5f,  0.5f, -0.5f, // 3
        -0.5f, -0.5f,  0.5f, // 4
         0.5f, -0.5f,  0.5f, // 5
         0.5f,  0.5f,  0.5f, // 6
        -0.5f,  0.5f,  0.5f  // 7
    };

    static unsigned int lightCubeIndices[] =
    {
        0,1,2,2,3,0,
        4,5,6,6,7,4,
        7,3,0,0,4,7,
        6,2,1,1,5,6,
        0,1,5,5,4,0,
        3,2,6,6,7,3
    };

    Light::Light(glm::vec3 color, glm::vec3 position)
        : GLObject(lightCubeVertices, sizeof(lightCubeVertices), GLVertexBufferLayout({3}),
                   lightCubeIndices, 36,
                   std::string(PROJ_RES_PATH).append("/Shaders/TestBlinnPhong/light.vert"),
                   std::string(PROJ_RES_PATH).append("/Shaders/TestBlinnPhong/light.frag"))
    {
        m_translation = position;
        m_scale = glm::vec3(0.1f);

        m_color = color;
        m_basicMaterial->ambient = glm::vec3(0.2f);
        m_basicMaterial->diffuse = color;
        m_basicMaterial->specular = glm::vec3(1.0f);
    }

    Light::~Light()
    {

    }

    void Light::onImGuiRender(const std::string& ObjectName)
    {
        const std::string objID("##" + std::to_string(m_uuid()));
        if (ImGui::CollapsingHeader((ObjectName + objID).c_str()))
        {
            ImGui::SeparatorText(std::string("Transforms" + objID).c_str());
            ImGui::DragFloat3(std::string("Rotation (Euler Angle)" + objID).c_str(), &m_rotation.x, 0.25f, -360.0f, 360.0f);
            ImGui::DragFloat3(std::string("Translation" + objID).c_str(), &m_translation.x, 0.25f, -100.0f, 100.0f);

            ImGui::SeparatorText(std::string("Attributes" + objID).c_str());
            ImGui::Checkbox(std::string("isVisible" + objID).c_str(), &m_isVisible);

            ImGui::SeparatorText(std::string("Material" + objID).c_str());
            ImGui::DragFloat3(std::string("Ambient" + objID).c_str(), &m_basicMaterial->ambient.r, 0.005f, 0.0f, 1.0f);
            ImGui::DragFloat3(std::string("Diffuse" + objID).c_str(), &m_basicMaterial->diffuse.r, 0.005f, 0.0f, 1.0f);
            ImGui::DragFloat3(std::string("Specular" + objID).c_str(), &m_basicMaterial->specular.r, 0.005f, 0.0f, 1.0f);
        }
    }
}