#include "Test/TestImgBlend.h"

#include "Core/Renderer.h"

#include "thirdParty/FileBrowser/ImGuiFileDialog.h"
#include "thirdParty/FileBrowser/ImGuiFileDialogConfig.h"

namespace test
{
	static std::string proj_res_path(PROJ_RES_PATH);
	static const char* blendModeStr[] = {
		"Normal", "Dissolve", "Darken", "Multiply",
		"ColorBurn", "LinearBurn", "DarkerColor",
		"Lighten", "Screen", "ColorDodge",
		"LinearDodge", "LighterColor", "Overlay",
		"SoftLight", "HardLight", "VividLight",
		"LinearLight", "PinLight", "HardMix",
		"Difference", "Exclusion", "Subtract",
		"Divide",
	};

	TestImgBlend::TestImgBlend()
	{
		// 启用混合
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		m_pCamera = std::make_unique<GLCore::Camera>(glm::vec3(0.0f, 0.0f, 2.0f));

		// 初始化数据
		float positions[] = {
			-0.5f + 0.2f, -0.5f + 0.2f, 0.0f, 0.0f, // 0
			0.5f + 0.2f, -0.5f + 0.2f, 1.0f, 0.0f,	// 1
			0.5f + 0.2f, 0.5f + 0.2f, 1.0f, 1.0f,	// 2
			-0.5f + 0.2f, 0.5f + 0.2f, 0.0f, 1.0f   // 3
		};
		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
		};
		
		m_pTex2D = std::make_unique<GLCore::GLObject>(
			positions, sizeof(positions), GLCore::GLVertexBufferLayout({2, 2}),
			indices, 6,
			std::string(proj_res_path + "/Shaders/TestImgBlend/resultColor.vert"),
			std::string(proj_res_path + "/Shaders/TestImgBlend/resultColor.frag")
		);
		m_pTex2D->setUniform("baseLayerImg", 0);
		m_pTex2D->setUniform("blendLayerImg", 1);

		m_blendColorLayer = glm::vec3(1.0f, 0.0f, 0.0f);
	}

	TestImgBlend::~TestImgBlend()
	{
		// 关闭混合
		GLCall(glDisable(GL_BLEND));
	}

	void TestImgBlend::onUpdate(float deltaTime, const Input& hardwareInput)
	{
		// 更新uniform变量
		glm::mat4 mvp = m_pCamera->getOrthoProjMat() * m_pCamera->getViewMat() * m_pTex2D->getModelMat();
		m_pTex2D->setUniform("u_MVP", mvp);
		m_pTex2D->setUniform("blendLayerColor", m_blendColorLayer);
		m_pTex2D->setUniform("useImageBlend", m_useImageBlend);
		m_pTex2D->setUniform("blendMode", static_cast<int>(m_blendMode));
		m_pTex2D->setUniform("opacity", m_blendOpacity);

		if (m_pBaseImgLayer)	m_pBaseImgLayer->bind(0);
		if (m_pBlendImgLayer)	m_pBlendImgLayer->bind(1);
	}

	void TestImgBlend::onRender()
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));
		{
			GLCore::Renderer renderer(nullptr);
			m_pTex2D->onRender(renderer);
		}
	}

	void TestImgBlend::onImGuiRender()
	{
		ImGui::Begin("Object##TestMultipleLights");
		std::string name = "Result Image";
		m_pTex2D->onImGuiRender(name);
		ImGui::End();

		ImGui::SeparatorText("Base Layer##TestImgBlend");
		if (m_pBaseImgLayer == nullptr)
		{
			if (ImGui::Button("Select Base Img##TestImgBlend"))
			{
				IGFD::FileDialogConfig config;
				config.path = std::string(proj_res_path + "/Textures");
				ImGuiFileDialog::Instance()
					->OpenDialog("ChooseBaseImg##TestImgBlend", 
								 "Select BaseImg##TestImgBlend", 
								 ".*", config);
			}
			if (ImGuiFileDialog::Instance()->Display("ChooseBaseImg##TestImgBlend")) {
				if (ImGuiFileDialog::Instance()->IsOk()) { // action if OK
					m_pBaseImgLayer = std::make_unique<GLCore::GLTexture>(
						ImGuiFileDialog::Instance()->GetFilePathName(),
						GLCore::TextureType::DiffuseMap,
						true
					);
					m_pBaseImgLayer->bind(0);
				}
				// close
				ImGuiFileDialog::Instance()->Close();
			}
		}
		else
		{
			if (ImGui::Button("Reset Base Img##TestImgBlend"))
			{
				m_pBaseImgLayer.reset();
			}
		}

		ImGui::SeparatorText("Blend Layer##TestImgBlend");
		ImGui::Checkbox("Use Image##TestImgBlend", &m_useImageBlend);
		if (m_useImageBlend)
		{
			if (m_pBlendImgLayer == nullptr)
			{
				if (ImGui::Button("Select Blend Img##TestImgBlend"))
				{
					IGFD::FileDialogConfig config;
					config.path = std::string(proj_res_path + "/Textures");
					ImGuiFileDialog::Instance()
						->OpenDialog("ChooseBlendImg##TestImgBlend",
									 "Select BlendImg##TestImgBlend",
									 ".*", config);
				}
				if (ImGuiFileDialog::Instance()->Display("ChooseBlendImg##TestImgBlend")) {
					if (ImGuiFileDialog::Instance()->IsOk()) { // action if OK
						m_pBlendImgLayer = std::make_unique<GLCore::GLTexture>(
							ImGuiFileDialog::Instance()->GetFilePathName(),
							GLCore::TextureType::DiffuseMap,
							true
						);
						m_pBlendImgLayer->bind(1);
					}
					// close
					ImGuiFileDialog::Instance()->Close();
				}
			}
			else
			{
				if (ImGui::Button("Reset Blend Img##TestImgBlend"))
				{
					m_pBlendImgLayer.reset();
				}
			}
		}
		else
		{
			ImGui::ColorPicker3("Blend Color##TestImgBlend", &m_blendColorLayer.r);
		}
		ImGui::DragFloat("Blend Layer Opacity##TestImgBlend", &m_blendOpacity, 0.005f, 0.0f, 1.0f);
		ImGui::Combo("Blend Mode##TestImgBlend",
					 reinterpret_cast<int*>(&m_blendMode),
					 blendModeStr, IM_ARRAYSIZE(blendModeStr));
		// LOG_DEBUG(std::format("Current blend mode:{}", blendModeStr[static_cast<int>(m_blendMode)]));
	}

	void TestImgBlend::processInput(const Input& hardware_input, float deltaTime) const
	{
		// 鼠标
		m_pCamera->processMouse(0, 0, hardware_input.mouseScrollYOffset);
	}
}
