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

	static auto myLerp = [](float a, float b, float f)
	{
		return a + f * (b - a);
	};

	TestSSAO::TestSSAO()
	{
		// 启用混合
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		// 启用深度缓冲
		GLCall(glEnable(GL_DEPTH_TEST));

		// 初始化摄像机
		m_pCamera = std::make_unique<GLCore::Camera>(glm::vec3(0.0f, 0.0f, 3.0f));

		// 初始化光源
		m_pLights.push_back(std::make_unique<GLCore::PointLight>(
			glm::vec3(0.2, 0.2, 0.7),
			glm::vec3(2, 4, -2), 0.09f, 0.032f)
		);

		// 初始化G-Buffer(Position, normal, albedo
		m_pGBufferFBO = std::make_unique<GLCore::GLFrameBuffer>();
		m_pGBufferFBO->addTextureAttachment(GLCore::FBAttachmentType::HDRColorAttachment, 1280, 768, GL_NEAREST);
		m_pGBufferFBO->addTextureAttachment(GLCore::FBAttachmentType::HDRColorAttachment, 1280, 768, GL_NEAREST);
		m_pGBufferFBO->addTextureAttachment(GLCore::FBAttachmentType::ColorAttachment, 1280, 768, GL_NEAREST);
		m_pGBufferFBO->addRBOAttachment(GLCore::FBAttachmentType::DepthAttachment, 1280, 768);
		m_pGBufferFBO->bindFBO();
		unsigned int attachments[3] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
		GLCall(glDrawBuffers(3, attachments));
		m_pGBufferFBO->unbindFBO();
		// 初始化G-Buffer Shader
		m_pGBufferShader = std::make_unique<GLCore::GLShader>(
			std::string(proj_res_path + "/Shaders/TestSSAO/gbObj.vert"),
			std::string(proj_res_path + "/Shaders/TestSSAO/gbObj.frag")
		);

		// 初始化SSAO半球采样核
		m_ssaoKernelSize = 64;
		m_ssaoSampleRadius = 0.5;
		m_ssaoSampleBias = 0.025;
		std::uniform_real_distribution<float> randomFloats(0.0, 1.0);	// [0.0, 1.0]的随机浮点数
		std::default_random_engine generator;
		for (unsigned int i = 0; i < m_ssaoKernelSize; ++i)
		{
			glm::vec3 sample(
				randomFloats(generator) * 2.0 - 1.0,
				randomFloats(generator) * 2.0 - 1.0,
				randomFloats(generator)
			);
			sample = glm::normalize(sample);
			sample *= randomFloats(generator);

			// 让样本更集中于半球球心
			float scale = static_cast<float>(i) / 64.0f;
			scale = myLerp(0.1f, 1.0f, scale * scale);
			sample *= scale;
			m_ssaoKernel.push_back(sample);
		}
		// 初始化SSAO半球采样核的随机旋转向量
		const unsigned int noiseSize = 16 * 16;
		for (unsigned int i = 0; i < noiseSize; ++i)
		{
			glm::vec3 noise(
				randomFloats(generator) * 2.0 - 1.0,
				randomFloats(generator) * 2.0 - 1.0,
				0.0f);
			m_ssaoNoise.push_back(noise);
		}
		// 初始化SSAO的随机纹理
		GLCall(glGenTextures(1, &m_ssaoNoiseTexture));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_ssaoNoiseTexture));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 16, 16, 0, GL_RGB, GL_FLOAT, &m_ssaoNoise[0]));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
		// 初始化SSAO的FBO
		m_pSsaoFBO = std::make_unique<GLCore::GLFrameBuffer>();
		m_pSsaoFBO->addTextureAttachment(GLCore::FBAttachmentType::SingleColorAttachment, 1280, 768, GL_NEAREST);
		// 初始化SSAO-blur的FBO
		m_pSsaoBlurFBO = std::make_unique<GLCore::GLFrameBuffer>();
		m_pSsaoBlurFBO->addTextureAttachment(GLCore::FBAttachmentType::SingleColorAttachment, 1280, 768, GL_NEAREST);
		// 初始化相关Shader
		m_pScrSsaoShader = std::make_unique<GLCore::GLShader>(
			std::string(proj_res_path + "/Shaders/TestSSAO/scr.vert"),
			std::string(proj_res_path + "/Shaders/TestSSAO/scrSsao.frag")
		);
		m_pScrSsaoShader->bind();
		m_pScrSsaoShader->setUniform("gPosition", 0);
		m_pScrSsaoShader->setUniform("gNormal", 1);
		m_pScrSsaoShader->setUniform("texNoise", 2);
		m_pScrSsaoShader->unbind();
		m_pScrSsaoBlurShader = std::make_unique<GLCore::GLShader>(
			std::string(proj_res_path + "/Shaders/TestSSAO/scr.vert"),
			std::string(proj_res_path + "/Shaders/TestSSAO/scrSsaoBlur.frag")
		);
		m_pScrSsaoBlurShader->bind();
		m_pScrSsaoBlurShader->setUniform("ssaoInput", 0);
		m_pScrSsaoBlurShader->unbind();

		// 初始化屏幕四边形
		m_pScrQuad = std::make_unique<GLCore::GLObject>(
			quadVertices, sizeof(quadVertices), GLCore::GLVertexBufferLayout({3, 2}),
			std::string(proj_res_path + "/Shaders/TestSSAO/scr.vert"),
			std::string(proj_res_path + "/Shaders/TestSSAO/scrSsao.frag")
		);
		// 初始化相关Shader
		m_pScrLightingPassShader = std::make_unique<GLCore::GLShader>(
			std::string(proj_res_path + "/Shaders/TestSSAO/scr.vert"),
			std::string(proj_res_path + "/Shaders/TestSSAO/scrLighting.frag")
		);
		m_pScrLightingPassShader->bind();
		m_pScrLightingPassShader->setUniform("gPosition", 0);
		m_pScrLightingPassShader->setUniform("gNormal", 1);
		m_pScrLightingPassShader->setUniform("gAlbedo", 2);
		m_pScrLightingPassShader->setUniform("ssao", 3);
		m_pScrLightingPassShader->unbind();
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
			m_proj = m_pCamera->getPerspectiveProjMat(1280, 768, 0.01f, 1000.0f);

			for (const auto& obj : m_pObjects)
			{
				obj->setUniform("projection", m_proj);
				obj->setUniform("view", m_view);
				obj->setUniform("model", obj->getModelMat());
				obj->onRender(renderer);
			}
		}
		m_pGBufferFBO->unbindFBO();

		// 2. 用G-Buffer渲染SSAO材质
		m_pSsaoFBO->bindFBO();
		GLCall(glClear(GL_COLOR_BUFFER_BIT));
		{
			GLCore::GLTexture* tex = m_pGBufferFBO->getTextures(GLCore::FBAttachmentType::HDRColorAttachment).at(0);
			tex->bind(0);
			tex = m_pGBufferFBO->getTextures(GLCore::FBAttachmentType::HDRColorAttachment).at(1);
			tex->bind(1);
			GLCall(glActiveTexture(GL_TEXTURE2));
			GLCall(glBindTexture(GL_TEXTURE_2D, m_ssaoNoiseTexture));

			m_pScrSsaoShader->bind();
			m_pScrSsaoShader->setUniform("kernelSize", static_cast<int>(m_ssaoKernelSize));
			m_pScrSsaoShader->setUniform("sampleRadius", m_ssaoSampleRadius);
			m_pScrSsaoShader->setUniform("sampleBias", m_ssaoSampleBias);
			for (unsigned int i = 0; i < m_ssaoKernelSize; ++i)
			{
				m_pScrSsaoShader->setUniform(std::format("samples[{}]", i), m_ssaoKernel.at(i));
			}
			m_pScrSsaoShader->setUniform("projection", m_proj);
			renderQuad(m_pScrSsaoShader.get());
			m_pScrSsaoShader->unbind();
		}
		m_pSsaoFBO->unbindFBO();

		// 3. 模糊SSAO结果, 去除噪点
		m_pSsaoBlurFBO->bindFBO();
		// GLCall(glClear(GL_COLOR_BUFFER_BIT));
		{
			GLCore::GLTexture* tex = m_pSsaoFBO->getTextures(GLCore::FBAttachmentType::SingleColorAttachment).at(0);
			tex->bind(0);
			m_pScrSsaoBlurShader->bind();
			renderQuad(m_pScrSsaoBlurShader.get());
			m_pScrSsaoBlurShader->unbind();
		}
		m_pSsaoBlurFBO->unbindFBO();

		// 4. 光照处理阶段
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
		{
			GLCore::GLTexture* tex = m_pGBufferFBO->getTextures(GLCore::FBAttachmentType::HDRColorAttachment).at(0);
			tex->bind(0);
			tex = m_pGBufferFBO->getTextures(GLCore::FBAttachmentType::HDRColorAttachment).at(1);
			tex->bind(1);
			tex = m_pGBufferFBO->getTextures(GLCore::FBAttachmentType::ColorAttachment).at(0);
			tex->bind(2);
			tex = m_pSsaoBlurFBO->getTextures(GLCore::FBAttachmentType::SingleColorAttachment).at(0);
			tex->bind(3);

			m_pScrLightingPassShader->bind();
			glm::vec3 lightPosView = glm::vec3(
				m_pCamera->getViewMat() * glm::vec4(m_pLights.at(0)->getTranslation(), 1.0)
			);
			m_pScrLightingPassShader->setUniform("light.Position", lightPosView);
			m_pScrLightingPassShader->setUniform("light.Color", m_pLights.at(0)->getBasicMaterial()->diffuse);
			m_pScrLightingPassShader->setUniform("light.Linear",
												 dynamic_cast<GLCore::PointLight*>(m_pLights[0].get())->getLinear());
			m_pScrLightingPassShader->setUniform("light.Quadratic",
												 dynamic_cast<GLCore::PointLight*>(m_pLights[0].get())->getQuadratic());
			renderQuad(m_pScrLightingPassShader.get());
			m_pScrLightingPassShader->unbind();
		}
		
		// 4.5 将G-Buffer的深度信息拷贝到默认帧缓冲上, 再渲染光源方块
		unsigned gBuffer = m_pGBufferFBO->getRBOs().at(0);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuffer);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // write to default framebuffer
		glBlitFramebuffer(0, 0, 1280, 768, 0, 0, 1280, 768, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		
		// 5. 前向渲染光源方块
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
					std::string(proj_res_path + "/Shaders/TestSSAO/gbObj.vert"),
					std::string(proj_res_path + "/Shaders/TestSSAO/gbObj.frag")
				));
			}
			// close
			ImGuiFileDialog::Instance()->Close();
		}

		ImGui::SeparatorText("SSAO setting##TestSSAO");
		ImGui::DragFloat("sampleRadius##TestSSAO", &m_ssaoSampleRadius, 0.05, 0, 1);
		ImGui::DragFloat("sampleBias##TestSSAO", &m_ssaoSampleBias, 0.025, 0, 1);

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
		GLint swizzleR[4] = {GL_RED, GL_RED, GL_RED, GL_RED};
		GLCore::GLTexture* tex = nullptr;

		ImGui::Begin("Position##TestSSAO");
		tex = m_pGBufferFBO->getTextures(GLCore::FBAttachmentType::HDRColorAttachment).at(0);
		tex->bind(0);
		glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzle);
		ImGui::Image((void*)(intptr_t)tex->getID(), ImVec2(1280 * 0.5, 768 * 0.5));
		ImGui::End();

		ImGui::Begin("Normal##TestSSAO");
		tex = m_pGBufferFBO->getTextures(GLCore::FBAttachmentType::HDRColorAttachment).at(1);
		tex->bind(0);
		glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzle);
		ImGui::Image((void*)(intptr_t)tex->getID(), ImVec2(1280 * 0.5, 768 * 0.5));
		ImGui::End();

		ImGui::Begin("Albedo##TestSSAO");
		tex = m_pGBufferFBO->getTextures(GLCore::FBAttachmentType::ColorAttachment).at(0);
		tex->bind(0);
		glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzle);
		ImGui::Image((void*)(intptr_t)tex->getID(), ImVec2(1280 * 0.5, 768 * 0.5));
		ImGui::End();

		ImGui::Begin("Noise##TestSSAO");
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_ssaoNoiseTexture);
		glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzle);
		ImGui::Image((void*)(intptr_t)m_ssaoNoiseTexture, ImVec2(1280 * 0.5, 768 * 0.5));
		ImGui::End();

		ImGui::Begin("SSAO##TestSSAO");
		tex = m_pSsaoFBO->getTextures(GLCore::FBAttachmentType::SingleColorAttachment).at(0);
		tex->bind(0);
		glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleR);
		ImGui::Image((void*)(intptr_t)tex->getID(), ImVec2(1280 * 0.5, 768 * 0.5));
		ImGui::End();

		ImGui::Begin("SSAO-Blur##TestSSAO");
		tex = m_pSsaoBlurFBO->getTextures(GLCore::FBAttachmentType::SingleColorAttachment).at(0);
		tex->bind(0);
		glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleR);
		ImGui::Image((void*)(intptr_t)tex->getID(), ImVec2(1280 * 0.5, 768 * 0.5));
		ImGui::End();
	}

	void TestSSAO::renderQuad(const GLCore::GLShader* shader) const
	{
		GLCore::Renderer renderer(nullptr);
		const GLCore::ModelData* data = m_pScrQuad->getModelData();
		renderer.draw(*data->pRaw->VAO,
					  m_pScrQuad->getVBOSize() / m_pScrQuad->getVBOLayout()->getStride(),
					  *shader);
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
