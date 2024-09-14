#include "Test/TestBloom.h"
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

    static float planeVertices[] = {
        // positions            // normals         // texcoords
         25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
        -25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
        -25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,

         25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
        -25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,
         25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,  25.0f, 25.0f
    };

    static std::string proj_res_path(PROJ_RES_PATH);

    TestBloom::TestBloom()
    {
        // 启用深度缓冲
        GLCall(glEnable(GL_DEPTH_TEST));

        m_selfSkyboxRender = false;
        m_enableHDR = false;

        // 相机配置
        m_pCamera->setCamaraPos(m_pCamera->getCameraPos() + glm::vec3(0, 0, 0));

        // 场景
        // 地板
        m_pObjects.push_back(
            std::make_unique<GLCore::GLObject>(
                planeVertices, sizeof(planeVertices),
                GLCore::GLVertexBufferLayout({3, 3, 2}),
                std::string(proj_res_path + "/Shaders/TestBloom/object.vert"),
                std::string(proj_res_path + "/Shaders/TestBloom/object.frag"),
                std::vector<GLCore::TextureDesc>({
                    {proj_res_path + "/Textures/wood.png", GLCore::TextureType::DiffuseMap, true}})));
        // 箱子
        m_pObjects.push_back(
            std::make_unique<GLCore::GLObject>(
                cubeVertices, sizeof(cubeVertices), GLCore::GLVertexBufferLayout({3, 3, 2}),
                std::string(proj_res_path + "/Shaders/TestBloom/object.vert"),
                std::string(proj_res_path + "/Shaders/TestBloom/object.frag"),
                std::vector<GLCore::TextureDesc>(
                    {
                        {proj_res_path + "/Textures/container2.png", GLCore::TextureType::DiffuseMap, true},
                        {proj_res_path + "/Textures/container2_specular.png", GLCore::TextureType::SpecularMap, true}
                    })));
        m_pObjects.back()->setScale(0.5, 0.5, 0.5);
        m_pObjects.back()->setTranslation(0, 1.5, 0);
        m_pObjects.push_back(
            std::make_unique<GLCore::GLObject>(
                cubeVertices, sizeof(cubeVertices), GLCore::GLVertexBufferLayout({3, 3, 2}),
                std::string(proj_res_path + "/Shaders/TestBloom/object.vert"),
                std::string(proj_res_path + "/Shaders/TestBloom/object.frag"),
                std::vector<GLCore::TextureDesc>(
                    {
                        {proj_res_path + "/Textures/container2.png", GLCore::TextureType::DiffuseMap, true},
                        {proj_res_path + "/Textures/container2_specular.png", GLCore::TextureType::SpecularMap, true}
                    })));
        m_pObjects.back()->setScale(0.5, 0.5, 0.5);
        m_pObjects.back()->setRotation(60, 60, 60);
        m_pObjects.back()->setTranslation(-2, 0, 0.75);
        m_pObjects.push_back(
            std::make_unique<GLCore::GLObject>(
                cubeVertices, sizeof(cubeVertices), GLCore::GLVertexBufferLayout({3, 3, 2}),
                std::string(proj_res_path + "/Shaders/TestBloom/object.vert"),
                std::string(proj_res_path + "/Shaders/TestBloom/object.frag"),
                std::vector<GLCore::TextureDesc>(
                    {
                        {proj_res_path + "/Textures/container2.png", GLCore::TextureType::DiffuseMap, true},
                        {proj_res_path + "/Textures/container2_specular.png", GLCore::TextureType::SpecularMap, true}
                    })));
        m_pObjects.back()->setScale(1.25, 1.25, 1.25);
        m_pObjects.back()->setRotation(23, 23, 23);
        m_pObjects.back()->setTranslation(0, 2.7, 2);
        m_pObjects.push_back(
            std::make_unique<GLCore::GLObject>(
                cubeVertices, sizeof(cubeVertices), GLCore::GLVertexBufferLayout({3, 3, 2}),
                std::string(proj_res_path + "/Shaders/TestBloom/object.vert"),
                std::string(proj_res_path + "/Shaders/TestBloom/object.frag"),
                std::vector<GLCore::TextureDesc>(
                    {
                        {proj_res_path + "/Textures/container2.png", GLCore::TextureType::DiffuseMap, true},
                        {proj_res_path + "/Textures/container2_specular.png", GLCore::TextureType::SpecularMap, true}
                    })));
        m_pObjects.back()->setRotation(60, 0, 60);
        m_pObjects.back()->setTranslation(-1, -1, 1.75);
        m_pObjects.push_back(
            std::make_unique<GLCore::GLObject>(
                cubeVertices, sizeof(cubeVertices), GLCore::GLVertexBufferLayout({3, 3, 2}),
                std::string(proj_res_path + "/Shaders/TestBloom/object.vert"),
                std::string(proj_res_path + "/Shaders/TestBloom/object.frag"),
                std::vector<GLCore::TextureDesc>(
                    {
                        {proj_res_path + "/Textures/container2.png", GLCore::TextureType::DiffuseMap, true},
                        {proj_res_path + "/Textures/container2_specular.png", GLCore::TextureType::SpecularMap, true}
                    })));
        m_pObjects.back()->setTranslation(-2, 0.75, -4.25);
        m_pObjects.push_back(
            std::make_unique<GLCore::GLObject>(
                cubeVertices, sizeof(cubeVertices), GLCore::GLVertexBufferLayout({3, 3, 2}),
                std::string(proj_res_path + "/Shaders/TestBloom/object.vert"),
                std::string(proj_res_path + "/Shaders/TestBloom/object.frag"),
                std::vector<GLCore::TextureDesc>(
                    {
                        {proj_res_path + "/Textures/container2.png", GLCore::TextureType::DiffuseMap, true},
                        {proj_res_path + "/Textures/container2_specular.png", GLCore::TextureType::SpecularMap, true}
                    })));
        m_pObjects.back()->setScale(0.5, 0.5, 0.5);
        m_pObjects.back()->setTranslation(2, 0, 0.25);
        // 灯光配置
        m_pLights.push_back(std::make_unique<GLCore::PointLight>(glm::vec3(15, 15, 15), glm::vec3(0, 0.5, 1.5), 0.09, 5));
        m_pLights.push_back(std::make_unique<GLCore::PointLight>(glm::vec3(5, 0, 0), glm::vec3(-4, 0.5, -3), 0.09, 5));
        m_pLights.push_back(std::make_unique<GLCore::PointLight>(glm::vec3(0, 0, 9), glm::vec3(3, 0, 0.25), 0.09, 5));
        m_pLights.push_back(std::make_unique<GLCore::PointLight>(glm::vec3(0, 8, 0), glm::vec3(-0.8, 2.5, -1), 0.09, 5));


        // FBO配置
        m_pFBO = std::make_unique<GLCore::GLFrameBuffer>();
        m_pFBO->addTextureAttachment(GLCore::FBAttachmentType::HDRColorAttachment, 1280, 768);
        m_pFBO->addTextureAttachment(GLCore::FBAttachmentType::HDRColorAttachment, 1280, 768);
        m_pFBO->addRBOAttachment(GLCore::FBAttachmentType::DepthAttachment, 1280, 768);

        // 高斯模糊 - shader
        m_pBlurShader = std::make_unique<GLCore::GLShader>(
            std::string(proj_res_path + "/Shaders/TestBloom/blur.vert"),
            std::string(proj_res_path + "/Shaders/TestBloom/blur.frag")
        );

        // 高斯模糊 - 乒乓FBO
        for (int i = 0; i < 2; ++i)
        {
            m_pBlurFBOs.push_back(std::make_unique<GLCore::GLFrameBuffer>());
            m_pBlurFBOs.back()->addTextureAttachment(GLCore::FBAttachmentType::HDRColorAttachment, 1280, 768);
        }

        // screenQuad配置
        m_pScreenQuad = std::make_unique<GLCore::GLObject>(
            quadVertices, sizeof(quadVertices), GLCore::GLVertexBufferLayout({3, 2}),
            std::string(proj_res_path + "/Shaders/TestBloom/screenQuad.vert"),
            std::string(proj_res_path + "/Shaders/TestBloom/screenQuad.frag")
        );
        m_pScreenQuadShader = std::make_unique<GLCore::GLShader>(
            std::string(proj_res_path + "/Shaders/TestBloom/screenQuad.vert"),
            std::string(proj_res_path + "/Shaders/TestBloom/screenQuad.frag")
        );
    }

    TestBloom::~TestBloom()
    {
        // 关闭混合和深度缓冲
        GLCall(glDisable(GL_DEPTH_TEST));
    }

    void TestBloom::onUpdate(float deltaTime, const Input& hardwareInput)
    {
        TestMultipleLights::onUpdate(deltaTime, hardwareInput);
    }

    void TestBloom::onRender()
    {
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        // 渲染HDR, 亮度图至FBO中
        GLCall(glEnable(GL_FRAMEBUFFER_SRGB));
        m_pFBO->bindFBO();
        // tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
        unsigned int attachments[2] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
        glDrawBuffers(2, attachments);
        TestMultipleLights::onRender();
        m_pFBO->unbindFBO();
        GLCall(glDisable(GL_FRAMEBUFFER_SRGB));

        // 高斯模糊
        renderHDRQuad(0);

        // 渲染最终结果
        if (m_enableHDR)
            renderHDRQuad(1);
        else
            TestMultipleLights::onRender();
    }

    void TestBloom::onImGuiRender()
    {
        ImGui::SeparatorText("Toggles##TestBloom");
        ImGui::Checkbox("Enable Bloom##TestBloom", &m_enableHDR);
        ImGui::DragFloat("Exposure##TestBloom", &m_exposure, 0.005f, 0.0f, 5.0f);

        ImGui::Begin("HDR Map##TestBloom");
        GLCore::GLTexture* tex = m_pFBO->getTextures(GLCore::FBAttachmentType::HDRColorAttachment).at(0);
        tex->bind(0);
        GLint swizzle[4] = {
            GL_RED,		    // Shader Red   channel source = Texture Red
            GL_GREEN,		// Shader Green channel source = Texture Green
            GL_BLUE,		// Shader Blue  channel source = Texture Blue
            GL_ALPHA		// Shader Alpha channel source = Texture Alpha
        };
        glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzle);
        tex->unbind();
        ImGui::Image((void*)(intptr_t)tex->getID(), ImVec2(1280 * 0.5, 768 * 0.5));
        ImGui::End();

        ImGui::Begin("Bright Map##TestBloom");
        tex = m_pFBO->getTextures(GLCore::FBAttachmentType::HDRColorAttachment).at(1);
        tex->bind(0);
        glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzle);
        tex->unbind();
        ImGui::Image((void*)(intptr_t)tex->getID(), ImVec2(1280 * 0.5, 768 * 0.5));
        ImGui::End();

        ImGui::Begin("Blured Map##TestBloom");
        tex = m_pBlurFBOs[0]->getTextures(GLCore::FBAttachmentType::HDRColorAttachment).at(0);
        tex->bind(0);
        glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzle);
        tex->unbind();
        ImGui::Image((void*)(intptr_t)tex->getID(), ImVec2(1280 * 0.5, 768 * 0.5));
        ImGui::End();

        TestMultipleLights::onImGuiRender();
    }

    void TestBloom::renderHDRQuad(int step) const
    {
        {
            GLCore::Renderer renderer(nullptr);
            const GLCore::ModelData* data = m_pScreenQuad->getModelData();
            if (step == 0)
            {
                bool horizontal = true;
            	bool first_iteration = true;
                int amount = 10;
                m_pBlurShader->bind();
                for (int i = 0; i < amount; ++i)
                {
                    m_pBlurShader->setUniform("horizontal", horizontal);
                    m_pBlurShader->setUniform("image", 0);

                    m_pBlurFBOs[horizontal ? 1 : 0]->bindFBO();
                    GLCore::GLTexture* tex = nullptr;
                    if (first_iteration)
                    {
                        tex = m_pFBO->getTextures(GLCore::FBAttachmentType::HDRColorAttachment).at(1);
                        first_iteration = false;
                    }
                    else
                    {
                        tex = m_pBlurFBOs[!horizontal ? 1 : 0]->getTextures(GLCore::FBAttachmentType::HDRColorAttachment).at(0);
                    }
                    tex->bind(0);
                    renderer.draw(*data->pRaw->VAO,
                                  m_pScreenQuad->getVBOSize() / m_pScreenQuad->getVBOLayout()->getStride(),
                                  *m_pBlurShader);
                    horizontal = !horizontal;
                }
                m_pBlurFBOs[horizontal]->unbindFBO();
                m_pBlurShader->unbind();
            }
            else if (step == 1)
            {
                m_pScreenQuadShader->bind();
                m_pScreenQuadShader->setUniform("hdrScene", 0);
                m_pScreenQuadShader->setUniform("bloomBlur", 1);
                m_pScreenQuadShader->setUniform("exposure", m_exposure);
                GLCore::GLTexture* hdrTex = m_pFBO->getTextures(GLCore::FBAttachmentType::HDRColorAttachment).at(0);
                GLCore::GLTexture* blurTex = m_pBlurFBOs[0]->getTextures(GLCore::FBAttachmentType::HDRColorAttachment).at(0);
                hdrTex->bind(0);
                blurTex->bind(1);
                renderer.draw(*data->pRaw->VAO,
                              m_pScreenQuad->getVBOSize() / m_pScreenQuad->getVBOLayout()->getStride(),
                              *m_pScreenQuadShader);
                hdrTex->unbind();
            	blurTex->unbind();
                m_pScreenQuadShader->unbind();
            }
        }
    }
}
