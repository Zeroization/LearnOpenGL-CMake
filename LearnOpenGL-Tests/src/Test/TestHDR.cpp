#include "Test/TestHDR.h"
#include "Render/Light/PointLight.h"

namespace test
{
	static float cubeVertices[] = {
        // back face
        -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
         1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
         1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
         1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
        -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
        -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
        // front face
        -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
         1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
         1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
         1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
        -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
        -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
        // left face
        -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
        -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
        -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
        -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
        -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
        -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
        // right face
         1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
         1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
         1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
         1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
         1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
         1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
         // bottom face
         -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
          1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
          1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
          1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
         -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
         -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
         // top face
         -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
          1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
          1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
          1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
         -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
         -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
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

	TestHDR::TestHDR()
	{
        // 启用深度缓冲
        GLCall(glEnable(GL_DEPTH_TEST));

		m_selfSkyboxRender = false;
		m_enableHDR = true;

        // 相机配置
        m_pCamera->setCamaraPos(m_pCamera->getCameraPos() + glm::vec3(0, 0, 25));

        // 管道场景
        m_pObjects.push_back(std::make_unique<GLCore::GLObject>(
            cubeVertices, sizeof(cubeVertices), GLCore::GLVertexBufferLayout({3, 3, 2}),
            std::string(proj_res_path + "/Shaders/TestMultipleLights/object.vert"),
            std::string(proj_res_path + "/Shaders/TestMultipleLights/object.frag"),
            std::vector<GLCore::TextureDesc>({
                {proj_res_path + "/Textures/wood.png", GLCore::TextureType::DiffuseMap, true}
                                             })
        ));
        m_pObjects.at(0)->setScale(2.0f, 2.0f, 30.0f);

		// 灯光配置
        m_pLights.push_back(std::make_unique<GLCore::PointLight>(glm::vec3(1, 1, 1), glm::vec3(0, 0, -30), 0.09, 0.14));
        m_pLights.back()->getBasicMaterial()->setAmbient(glm::vec3(20, 20, 20));

        m_pLights.push_back(std::make_unique<GLCore::PointLight>(glm::vec3(1, 0, 0), glm::vec3(2.25, -2.5, 11)));
        m_pLights.back()->getBasicMaterial()->setAmbient(glm::vec3(0, 0, 0));

        m_pLights.push_back(std::make_unique<GLCore::PointLight>(glm::vec3(0, 1, 0), glm::vec3(-2.25, -2.5, 15.3)));
        m_pLights.back()->getBasicMaterial()->setAmbient(glm::vec3(0, 0, 0));

        m_pLights.push_back(std::make_unique<GLCore::PointLight>(glm::vec3(0, 0, 1), glm::vec3(0, -3.25, 20)));
        m_pLights.back()->getBasicMaterial()->setAmbient(glm::vec3(0, 0, 0));

        // FBO配置
        m_pHDRFBO = std::make_unique<GLCore::GLFrameBuffer>();
        m_pHDRFBO->addTextureAttachment(GLCore::FBAttachmentType::HDRColorAttachment, 1280, 768);
        m_pHDRFBO->addRBOAttachment(GLCore::FBAttachmentType::DepthAttachment, 1280, 768);

        // screenQuad配置
        m_pScreenQuad = std::make_unique<GLCore::GLObject>(
            quadVertices, sizeof(quadVertices), GLCore::GLVertexBufferLayout({3, 2}),
            std::string(proj_res_path + "/Shaders/TestHDR/HDR.vert"),
            std::string(proj_res_path + "/Shaders/TestHDR/HDR.frag")
        );
        GLCore::GLTexture* tex = m_pHDRFBO->getTextures(GLCore::FBAttachmentType::HDRColorAttachment).at(0);
        m_pScreenQuad->addTexture(tex);
	}

	TestHDR::~TestHDR()
	{
        // 关闭混合和深度缓冲
        GLCall(glDisable(GL_DEPTH_TEST));
	}

	void TestHDR::onUpdate(float deltaTime, const Input& hardwareInput)
	{
		TestMultipleLights::onUpdate(deltaTime, hardwareInput);
	}

	void TestHDR::onRender()
	{
        // 渲染HDR至FBO中
        m_pHDRFBO->bindFBO();
		TestMultipleLights::onRender();
        m_pHDRFBO->unbindFBO();

        if (m_enableHDR)
            renderHDRQuad();
        else
            TestMultipleLights::onRender();
	}

	void TestHDR::onImGuiRender()
	{
		ImGui::SeparatorText("Toggles##TestHDR");
		ImGui::Checkbox("Enable HDR##TestHDR", &m_enableHDR);
        ImGui::DragFloat("Exposure##TestHDR", &m_exposure, 0.005f, 0.0f, 5.0f);

        ImGui::Begin("HDR Map##TestHDR");
        GLCore::GLTexture* tex = m_pHDRFBO->getTextures(GLCore::FBAttachmentType::HDRColorAttachment).at(0);
        // Texture must be bound first
        tex->bind(1);
        GLint swizzle[4] = {
            GL_RED,		    // Shader Red   channel source = Texture Red
            GL_GREEN,		// Shader Green channel source = Texture Green
            GL_BLUE,		// Shader Blue  channel source = Texture Blue
            GL_ALPHA		// Shader Alpha channel source = Texture Alpha
        };
        glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzle);
        tex->unbind();
        ImGui::Image((void*)(intptr_t)tex->getID(), ImVec2(128.0f * 4, 76.8f * 4));
        ImGui::End();

        TestMultipleLights::onImGuiRender();
	}

	void TestHDR::renderHDRQuad()
	{
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
        {
            GLCore::Renderer renderer(nullptr);
            m_pScreenQuad->setUniform("hdrBuffer", 0);
            m_pScreenQuad->setUniform("exposure", m_exposure);
            m_pScreenQuad->onRender(renderer);
        }
	}
}
