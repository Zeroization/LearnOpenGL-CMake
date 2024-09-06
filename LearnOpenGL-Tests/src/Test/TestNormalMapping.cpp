#include "Test/TestNormalMapping.h"

#include "Render/Light/PointLight.h"

namespace test
{
	static std::string proj_res_path(PROJ_RES_PATH);

	TestNormalMapping::TestNormalMapping()
	{
		// 启用混合
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		// 启用深度缓冲
		GLCall(glEnable(GL_DEPTH_TEST));

		m_pCamera = std::make_unique<GLCore::Camera>(glm::vec3(0.0f, 0.0f, 3.0f));

		// 计算TBN
		// positions
		glm::vec3 pos1(-1.0, 1.0, 0.0);
		glm::vec3 pos2(-1.0, -1.0, 0.0);
		glm::vec3 pos3(1.0, -1.0, 0.0);
		glm::vec3 pos4(1.0, 1.0, 0.0);
		// texCoords
		glm::vec2 uv1(0.0, 1.0);
		glm::vec2 uv2(0.0, 0.0);
		glm::vec2 uv3(1.0, 0.0);
		glm::vec2 uv4(1.0, 1.0);
		// normal
		glm::vec3 nm(0.0, 0.0, 1.0);

		// 三角形1,2,3的边和UV
		glm::vec3 edge1 = pos2 - pos1;
		glm::vec3 edge2 = pos3 - pos1;
		glm::vec2 deltaUV1 = uv2 - uv1;
		glm::vec2 deltaUV2 = uv3 - uv1;
		float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
		glm::vec3 tangent1, bitangent1;
		tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
		tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
		tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
		tangent1 = glm::normalize(tangent1);
		bitangent1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
		bitangent1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
		bitangent1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
		bitangent1 = glm::normalize(bitangent1);

		// 三角形1, 3, 4的边和UV
		edge1 = pos3 - pos1;
		edge2 = pos4 - pos1;
		deltaUV1 = uv3 - uv1;
		deltaUV2 = uv4 - uv1;
		f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
		glm::vec3 tangent2, bitangent2;
		tangent2.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
		tangent2.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
		tangent2.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
		tangent2 = glm::normalize(tangent2);
		bitangent2.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
		bitangent2.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
		bitangent2.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
		bitangent2 = glm::normalize(bitangent2);

		float quadVertices[] = {
			// positions            // normal         // texcoords  // tangent                          // bitangent
			pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
			pos2.x, pos2.y, pos2.z, nm.x, nm.y, nm.z, uv2.x, uv2.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
			pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,

			pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
			pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
			pos4.x, pos4.y, pos4.z, nm.x, nm.y, nm.z, uv4.x, uv4.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z
		};

		// 添加砖墙
		m_pObjects.push_back(
			std::make_unique<GLCore::GLObject>(
				quadVertices, sizeof(quadVertices),
				GLCore::GLVertexBufferLayout({3, 3, 2, 3, 3}),
				std::string(proj_res_path + "/Shaders/TestNormalMapping/brick.vert"),
				std::string(proj_res_path + "/Shaders/TestNormalMapping/brick.frag"),
				std::vector<GLCore::TextureDesc>({
					{proj_res_path + "/Textures/brickwall/bricks2_diffuse.jpg", GLCore::TextureType::DiffuseMap, true},
					{proj_res_path + "/Textures/brickwall/bricks2_normal.jpg", GLCore::TextureType::NormalMap, true},
					{proj_res_path + "/Textures/brickwall/bricks2_disp.jpg", GLCore::TextureType::HeightMap, true}
												 })
			));

		// 添加点光源
		m_pLights.push_back(std::make_unique<GLCore::PointLight>());
	}

	TestNormalMapping::~TestNormalMapping()
	{
		// 关闭混合和深度缓冲
		GLCall(glDisable(GL_BLEND));
		GLCall(glDisable(GL_DEPTH_TEST));
		m_pCamera.reset();
		for (auto& p_object : m_pObjects)
		{
			p_object.reset();
		}
		for (auto& p_light : m_pLights)
		{
			p_light.reset();
		}
	}

	void TestNormalMapping::onUpdate(float deltaTime, const Input& hardwareInput)
	{
		// 处理硬件输入
		processInput(hardwareInput, deltaTime);

		// 处理Shader
		m_view = m_pCamera->getViewMat();
		m_proj = m_pCamera->getPerspectiveProjMat(hardwareInput.screenWidth, hardwareInput.screenHeight);

		if (m_pLights.empty())
		{
			m_pLights.push_back(std::make_unique<GLCore::PointLight>());
		}

		auto& light = m_pLights[0];
		light->setUniform("u_MVP", m_proj * m_view * light->getModelMat());
		light->setUniform("u_LightColor", light->getBasicMaterial().diffuse);

		auto& object = m_pObjects[0];
		object->setUniform("model", object->getModelMat());
		object->setUniform("view", m_view);
		object->setUniform("projection", m_proj);
		object->setUniform("lightPos", light->getTranslation());
		object->setUniform("viewPos", m_pCamera->getCameraPos());
		object->setUniform("diffuseMap", 0);
		object->setUniform("normalMap", 1);
		object->setUniform("depthMap", 2);
		object->setUniform("useNormalMap", m_useNormalMap);
		object->setUniform("useParallaxMap", m_useParallaxMap);
		object->setUniform("heightScale", 0.1f);
	}

	void TestNormalMapping::onRender()
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		{
			GLCore::Renderer renderer(nullptr);
			for (auto& m_pObject : m_pObjects)
			{
				m_pObject->onRender(renderer);
			}
			for (auto& m_pLight : m_pLights)
			{
				m_pLight->onRender(renderer);
			}
		}
	}

	void TestNormalMapping::onImGuiRender()
	{
		ImGui::SeparatorText("Toggles##TestNormalMapping");
		ImGui::Checkbox("Enable NormalMapping##TestNormalMapping", &m_useNormalMap);
		ImGui::Checkbox("Enable ParallaxMapping##TestNormalMapping", &m_useParallaxMap);

		ImGui::Begin("Objects##TestNormalMapping");
		for (size_t i = 0; i < m_pObjects.size(); ++i)
		{
			std::string name = "BrickWall";
			m_pObjects.at(i)->onImGuiRender(name);
		}
		ImGui::End();

		ImGui::Begin("Lights##TestNormalMapping");
		for (size_t i = 0; i < m_pLights.size(); ++i)
		{
			m_pLights.at(i)->onImGuiRender(m_pLights.at(i)->getLightTypeString());
		}
		ImGui::End();
	}

	void TestNormalMapping::processInput(const Input& hardware_input, float deltaTime) const
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
