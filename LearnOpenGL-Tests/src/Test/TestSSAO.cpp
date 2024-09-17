#include "Test/TestSSAO.h"

#include "thirdParty/FileBrowser/ImGuiFileDialog.h"
#include "thirdParty/FileBrowser/ImGuiFileDialogConfig.h"

#include <random>

#include "Render/Light/PointLight.h"

namespace test
{
	static float cubeVertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	};

	static float quadVertices[] = {
		// positions        // texCoords
		-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,

		-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
		 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		 1.0f,  1.0f, 0.0f, 1.0f, 1.0f
	};

	static std::string proj_res_path(PROJ_RES_PATH);

	TestSSAO::TestSSAO()
	{
		// 启用混合
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		// 启用深度缓冲
		GLCall(glEnable(GL_DEPTH_TEST));

		// 初始化G-Buffer(Position, normal, albedo, specular)
		m_pGBufferFBO = std::make_unique<GLCore::GLFrameBuffer>();
		m_pGBufferFBO->addTextureAttachment(GLCore::FBAttachmentType::HDRColorAttachment, 1280, 768, GL_NEAREST);
		m_pGBufferFBO->addTextureAttachment(GLCore::FBAttachmentType::HDRColorAttachment, 1280, 768, GL_NEAREST);
		m_pGBufferFBO->addTextureAttachment(GLCore::FBAttachmentType::ColorAttachment, 1280, 768, GL_NEAREST);
		m_pGBufferFBO->addTextureAttachment(GLCore::FBAttachmentType::ColorAttachment, 1280, 768, GL_NEAREST);
		m_pGBufferFBO->addRBOAttachment(GLCore::FBAttachmentType::DepthAttachment, 1280, 768);
		m_pGBufferFBO->bindFBO();
		unsigned int attachments[4] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3};
		GLCall(glDrawBuffers(4, attachments));
		m_pGBufferFBO->unbindFBO();

		// 初始化G-Buffer Shader
		m_pGBufferShader = std::make_unique<GLCore::GLShader>(
			std::string(proj_res_path + "/Shaders/TestDR/gbObj.vert"),
			std::string(proj_res_path + "/Shaders/TestDR/gbObj.frag")
		);

		// 初始化摄像机
		m_pCamera = std::make_unique<GLCore::Camera>(glm::vec3(0.0f, 0.0f, 3.0f));

		// 初始化屏幕四边形
		m_pScrQuad = std::make_unique<GLCore::GLObject>(
			quadVertices, sizeof(quadVertices), GLCore::GLVertexBufferLayout({3, 2}),
			std::string(proj_res_path + "/Shaders/TestDR/scrQuad.vert"),
			std::string(proj_res_path + "/Shaders/TestDR/scrQuad.frag")
		);
		m_pScrQuad->addTexture(m_pGBufferFBO->getTextures(GLCore::FBAttachmentType::HDRColorAttachment).at(0));
		m_pScrQuad->addTexture(m_pGBufferFBO->getTextures(GLCore::FBAttachmentType::HDRColorAttachment).at(1));
		m_pScrQuad->addTexture(m_pGBufferFBO->getTextures(GLCore::FBAttachmentType::ColorAttachment).at(0));
		m_pScrQuad->addTexture(m_pGBufferFBO->getTextures(GLCore::FBAttachmentType::ColorAttachment).at(1));

		// 初始化光源
		static std::random_device s_seeder;
		static auto seed = s_seeder.entropy() ? s_seeder() : time(nullptr);
		static std::mt19937 s_engine(seed);
		static std::uniform_int_distribution<long> s_distribution;

		const unsigned int NR_LIGHTS = 32;
		for (unsigned int i = 0; i < NR_LIGHTS; ++i)
		{
			float xPos = static_cast<float>(((s_distribution(s_engine) % 100) / 100.0) * 6.0 - 3.0);
			float yPos = static_cast<float>(((s_distribution(s_engine) % 100) / 100.0) * 6.0 - 4.0);
			float zPos = static_cast<float>(((s_distribution(s_engine) % 100) / 100.0) * 6.0 - 3.0);

			float rColor = static_cast<float>(((s_distribution(s_engine) % 100) / 200.0f) + 0.5);
			float gColor = static_cast<float>(((s_distribution(s_engine) % 100) / 200.0f) + 0.5);
			float bColor = static_cast<float>(((s_distribution(s_engine) % 100) / 200.0f) + 0.5);

			m_pLights.push_back(std::make_unique<GLCore::PointLight>(
				glm::vec3(rColor, gColor, bColor), 
				glm::vec3(xPos, yPos, zPos), 0.7, 1.8)
			);
		}
	}

	TestSSAO::~TestSSAO()
	{
		// 关闭混合和深度缓冲
		GLCall(glDisable(GL_BLEND));
		GLCall(glDisable(GL_DEPTH_TEST));
	}

	void TestSSAO::onUpdate(float deltaTime, const Input& hardwareInput)
	{
		// 处理硬件输入
		processInput(hardwareInput, deltaTime);

		// 处理Shader
		m_view = m_pCamera->getViewMat();
		m_proj = m_pCamera->getPerspectiveProjMat(1280, 768);
		for (const auto& m_pLight : m_pLights)
		{
			glm::mat4 model = m_pLight->getModelMat();
			m_pLight->setUniform("u_MVP", m_proj * m_view * model);
			m_pLight->setUniform("u_LightColor", m_pLight->getBasicMaterial()->diffuse);
		}
	}

	void TestSSAO::onRender()
	{
		// 1. 几何处理阶段
		m_pGBufferFBO->bindFBO();
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
		{
			GLCore::Renderer renderer(nullptr);
			m_view = m_pCamera->getViewMat();
			m_proj = m_pCamera->getPerspectiveProjMat(1280, 768);

			for (const auto& obj : m_pObjects)
			{
				obj->setUniform("projection", m_proj);
				obj->setUniform("view", m_view);
				obj->setUniform("model", obj->getModelMat());
				obj->onRender(renderer);
			}
		}
		m_pGBufferFBO->unbindFBO();

		// 2. 光照处理阶段
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
		{
			GLCore::Renderer renderer(nullptr);
			for (unsigned int i = 0; i < m_pLights.size(); ++i)
			{
				m_pScrQuad->setUniform(std::format("lights[{}].Position", i), m_pLights[i]->getTranslation());
				m_pScrQuad->setUniform(std::format("lights[{}].Color", i), m_pLights[i]->getColor());
				m_pScrQuad->setUniform(std::format("lights[{}].Linear", i),
									   dynamic_cast<GLCore::PointLight*>(m_pLights[i].get())->getLinear());
				m_pScrQuad->setUniform(std::format("lights[{}].Quadratic", i),
									   dynamic_cast<GLCore::PointLight*>(m_pLights[i].get())->getQuadratic());
				m_pScrQuad->setUniform(std::format("lights[{}].Radius", i),
									   dynamic_cast<GLCore::PointLight*>(m_pLights[i].get())->getVolumeRadius());
			}
			m_pScrQuad->setUniform("viewPos", m_pCamera->getCameraPos());
			m_pScrQuad->setUniform("gPosition", 0);
			m_pScrQuad->setUniform("gNormal", 1);
			m_pScrQuad->setUniform("gAlbedo", 2);
			m_pScrQuad->setUniform("gSpecular", 3);
			m_pScrQuad->onRender(renderer);
		}

		// 2.5 将G-Buffer的深度信息拷贝到默认帧缓冲上, 再渲染光源方块
		unsigned gBuffer = m_pGBufferFBO->getRBOs().at(0);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuffer);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // write to default framebuffer
		glBlitFramebuffer(0, 0, 1280, 768, 0, 0, 1280, 768, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// 3. 前向渲染光源方块
		{
			GLCore::Renderer renderer(nullptr);
			for (auto& light : m_pLights)
			{
				light->onRender(renderer);
			}
		}
	}

	void TestSSAO::onImGuiRender()
	{
		ImGui::SeparatorText("Render Target##TestSSAO");
		if (ImGui::Button("Add Model##TestSSAO"))
		{
			IGFD::FileDialogConfig config;
			config.path = std::string(PROJ_RES_PATH) + "/Models";
			ImGuiFileDialog::Instance()->OpenDialog("Choose A Model File##TestSSAO", "Please select model file", ".*", config);
		}
		if (ImGuiFileDialog::Instance()->Display("Choose A Model File##TestSSAO")) {
			if (ImGuiFileDialog::Instance()->IsOk()) { // action if OK
				std::string filePath = ImGuiFileDialog::Instance()->GetFilePathName();
				m_pObjects.push_back(std::make_unique<GLCore::GLObject>(
					filePath,
					std::string(proj_res_path + "/Shaders/TestDR/gbObj.vert"),
					std::string(proj_res_path + "/Shaders/TestDR/gbObj.frag")
				));
			}
			// close
			ImGuiFileDialog::Instance()->Close();
		}

		ImGui::Begin("Objects##TestSSAO");
		for (size_t i = 0; i < m_pObjects.size(); ++i)
		{
			std::string name = m_pObjects.at(i)->getDataType() == GLCore::ModelDataType::RAW ? "Wooden Box" : "Model";
			m_pObjects.at(i)->onImGuiRender(name);
			if (ImGui::Button(std::string("Delete##" + m_pObjects.at(i)->getUUID()).c_str()))
			{
				m_pObjects.at(i).reset();
				m_pObjects.erase(std::begin(m_pObjects) + i);
			}
		}
		ImGui::End();

		ImGui::Begin("Lights##TestSSAO");
		for (size_t i = 0; i < m_pLights.size(); ++i)
		{
			m_pLights.at(i)->onImGuiRender(m_pLights.at(i)->getLightTypeString());
			if (ImGui::Button(std::string("Delete##" + m_pLights.at(i)->getUUID()).c_str()))
			{
				m_pLights.at(i)->releaseUniforms(m_pObjects);
				m_pLights.at(i).reset();
				m_pLights.erase(std::begin(m_pLights) + i);
			}
		}
		ImGui::End();

		GLint swizzle[4] = {
			GL_RED,		    // Shader Red   channel source = Texture Red
			GL_GREEN,		// Shader Green channel source = Texture Green
			GL_BLUE,		// Shader Blue  channel source = Texture Blue
			GL_ALPHA		// Shader Alpha channel source = Texture Alpha
		};
		GLCore::GLTexture* tex = nullptr;

		ImGui::Begin("Position Texture##TestSSAO");
		tex = m_pGBufferFBO->getTextures(GLCore::FBAttachmentType::HDRColorAttachment).at(0);
		tex->bind(0);
		glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzle);
		tex->unbind();
		ImGui::Image((void*)(intptr_t)tex->getID(), ImVec2(1280 * 0.5, 768 * 0.5));
		ImGui::End();

		ImGui::Begin("Normal Texture##TestSSAO");
		tex = m_pGBufferFBO->getTextures(GLCore::FBAttachmentType::HDRColorAttachment).at(1);
		tex->bind(0);
		glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzle);
		tex->unbind();
		ImGui::Image((void*)(intptr_t)tex->getID(), ImVec2(1280 * 0.5, 768 * 0.5));
		ImGui::End();

		ImGui::Begin("Albedo Texture##TestSSAO");
		tex = m_pGBufferFBO->getTextures(GLCore::FBAttachmentType::ColorAttachment).at(0);
		tex->bind(0);
		glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzle);
		tex->unbind();
		ImGui::Image((void*)(intptr_t)tex->getID(), ImVec2(1280 * 0.5, 768 * 0.5));
		ImGui::End();

		ImGui::Begin("Specular Texture##TestSSAO");
		tex = m_pGBufferFBO->getTextures(GLCore::FBAttachmentType::ColorAttachment).at(1);
		tex->bind(0);
		glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzle);
		tex->unbind();
		ImGui::Image((void*)(intptr_t)tex->getID(), ImVec2(1280 * 0.5, 768 * 0.5));
		ImGui::End();
	}

	void TestSSAO::processInput(const Input& hardware_input, float deltaTime) const
	{
		// 鼠标
		m_pCamera->processMouse(hardware_input.mouseMovXOffset, hardware_input.mouseMovYOffset,
								hardware_input.mouseScrollYOffset);
		// 键盘
		if (hardware_input.keyboardInput == GLFW_KEY_W)
		{
			m_pCamera->processKeyboard(GLCore::Camera::CameraMovDir::FORWARD, deltaTime);
		}
		if (hardware_input.keyboardInput == GLFW_KEY_S)
		{
			m_pCamera->processKeyboard(GLCore::Camera::CameraMovDir::BACKWARD, deltaTime);
		}
		if (hardware_input.keyboardInput == GLFW_KEY_A)
		{
			m_pCamera->processKeyboard(GLCore::Camera::CameraMovDir::LEFT, deltaTime);
		}
		if (hardware_input.keyboardInput == GLFW_KEY_D)
		{
			m_pCamera->processKeyboard(GLCore::Camera::CameraMovDir::RIGHT, deltaTime);
		}
		if (hardware_input.keyboardInput == GLFW_KEY_SPACE)
		{
			m_pCamera->processKeyboard(GLCore::Camera::CameraMovDir::UP, deltaTime);
		}
		if (hardware_input.keyboardInput == GLFW_KEY_LEFT_CONTROL)
		{
			m_pCamera->processKeyboard(GLCore::Camera::CameraMovDir::DOWN, deltaTime);
		}
	}
}
