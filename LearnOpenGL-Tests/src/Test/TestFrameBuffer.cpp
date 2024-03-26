#include "Test/TestFrameBuffer.h"

namespace test
{
	TestFrameBuffer::TestFrameBuffer()
		: TestMultipleLights(), m_postProcessParam(0)
	{
        // 后处理矩形的NDC坐标
        float quadVertices[] = {
            // positions   // texCoords
            -1.0f,  1.0f,  0.0f, 1.0f,
            -1.0f, -1.0f,  0.0f, 0.0f,
             1.0f, -1.0f,  1.0f, 0.0f,

            -1.0f,  1.0f,  0.0f, 1.0f,
             1.0f, -1.0f,  1.0f, 0.0f,
             1.0f,  1.0f,  1.0f, 1.0f
        };

        m_pScreenQuad = std::make_unique<GLCore::GLObject>(quadVertices, sizeof(quadVertices), GLCore::GLVertexBufferLayout({2, 2}),
													std::string(PROJ_RES_PATH) + "/Shaders/TestFrameBuffer/quad.vert",
													std::string(PROJ_RES_PATH) + "/Shaders/TestFrameBuffer/quad.frag");
        m_pFBO = std::make_unique<GLCore::GLFrameBuffer>();
        m_pFBO->addTextureAttachment(GLCore::FBAttachmentType::ColorAttachment, 1024, 768);
        m_pFBO->addRBOAttachment(GLCore::FBAttachmentType::DepthStencilAttachment, 1024, 768);
	}

	void TestFrameBuffer::onUpdate(float deltaTime, const Input& hardwareInput)
	{
		TestMultipleLights::onUpdate(deltaTime, hardwareInput);

        m_pScreenQuad->setUniform("u_PostProcessParam", m_postProcessParam);
	}

	void TestFrameBuffer::onRender()
	{
        // 后处理Pass
        m_pFBO->bindFBO();
        GLCall(glEnable(GL_DEPTH_TEST));
		TestMultipleLights::onRender();
        m_pFBO->unbindFBO();

        // 渲染之前Pass的结果
        GLCall(glDisable(GL_DEPTH_TEST));
        {
            GLCore::Renderer renderer(nullptr);
            m_pFBO->bindTextures();
            m_pScreenQuad->setUniform("u_ScreenTexture", 0);
            m_pScreenQuad->onRender(renderer);
        }
	}

	void TestFrameBuffer::onImGuiRender()
	{
        ImGui::SeparatorText("后处理##TestFrameBuffer");
        if (ImGui::Button("默认效果##TestFrameBuffer"))
        {
            m_postProcessParam = 0;
        }
        if (ImGui::Button("反色##TestFrameBuffer"))
        {
            m_postProcessParam = 1;
        }
        if (ImGui::Button("灰度图##TestFrameBuffer"))
        {
            m_postProcessParam = 2;
        }
        if (ImGui::Button("锐化##TestFrameBuffer"))
        {
            m_postProcessParam = 3;
        }
        if (ImGui::Button("模糊##TestFrameBuffer"))
        {
            m_postProcessParam = 4;
        }
        if (ImGui::Button("边缘检测##TestFrameBuffer"))
        {
            m_postProcessParam = 5;
        }

		TestMultipleLights::onImGuiRender();
	}
}
