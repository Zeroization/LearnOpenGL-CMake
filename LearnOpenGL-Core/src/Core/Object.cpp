#include "Core/Object.h"

#include "Utils/AssimpGLMHelper.hpp"

#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/dual_quaternion.hpp"


namespace GLCore
{
	GLObject::GLObject(float vertices[], size_t verticesSize, const GLVertexBufferLayout& vertLayout,
					   const std::string& vertPath, const std::string& fragPath,
					   const std::vector<TextureDesc>& textureDescList)
		: m_modelDataType(ModelDataType::RAW), m_isVisible(true)
	{
		m_modelData.pRaw = std::make_unique<RawModelData>();

		m_modelData.pRaw->verticesCache = vertices;
		m_modelData.pRaw->verticesSize = verticesSize;
		m_modelData.pRaw->indicesCache = nullptr;
		m_modelData.pRaw->indicesCount = 0;
		m_modelData.pRaw->VBO = std::make_unique<GLVertexBuffer>(vertices, verticesSize);
		m_modelData.pRaw->VBLayout = std::make_unique<GLVertexBufferLayout>(vertLayout);
		m_modelData.pRaw->VAO = std::make_unique<GLVertexArray>();
		m_modelData.pRaw->VAO->addVBO(*m_modelData.pRaw->VBO, *m_modelData.pRaw->VBLayout);

		m_material = std::make_unique<Material>(vertPath, fragPath, textureDescList);
		m_color = glm::vec3(1.0f, 0.0f, 1.0f);
		m_basicMaterial = m_material->getBasicMaterial();
		m_basicMaterial->ambient = glm::vec3(0.09f, 0.09f, 0.09f);
		m_basicMaterial->diffuse = m_color;
		m_basicMaterial->specular = glm::vec3(0.5f, 0.5f, 0.5f);
		m_basicMaterial->shininess = 0.1f;
		if (textureDescList.empty())
		{
			LOG_WARN("[GLObject](): load no texture !!!");
		}

		m_scale = glm::vec3(1.0f);
		m_translation = glm::vec3(0.0f);
		m_rotation = glm::vec3(0.0f);
	}

	GLObject::GLObject(float vertices[], size_t verticesSize, const GLVertexBufferLayout& vertLayout,
					   unsigned int* indices, int iCount,
					   const std::string& vertPath, const std::string& fragPath,
					   const std::vector<TextureDesc>& textureDescList)
		: GLObject(vertices, verticesSize, vertLayout, vertPath, fragPath, textureDescList)
	{
		m_modelData.pRaw->indicesCache = indices;
		m_modelData.pRaw->indicesCount = iCount;
		m_modelData.pRaw->IBO = std::make_unique<GLIndexBuffer>(indices, iCount);
	}

	GLObject::GLObject(const std::string& modelPath, const std::string& vertPath, const std::string& fragPath)
		: m_modelDataType(ModelDataType::CUSTOM), m_isVisible(true)
	{
		m_modelData.pCustom = std::make_unique<CustomModelData>();

		// 导入自定义模型
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(modelPath, aiProcess_Triangulate | aiProcess_FlipUVs |
												aiProcess_GenNormals);
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			LOG_CRITICAL("[GLObject](): can't load custom model by assimp!");
			__debugbreak();
		}
		m_modelData.pCustom->modelDir = modelPath.substr(0, modelPath.find_last_of('\\'));

		// 将Assimp数据结构转换为自定义数据结构
		processNode(scene->mRootNode, scene);

		// 处理自定义模型的动画
		if (scene->HasAnimations())
		{
			for (unsigned int i = 0; i < scene->mNumAnimations; ++i)
			{
				auto pAiAnimation = scene->mAnimations[i];
				std::string name = pAiAnimation->mName.C_Str();
				m_vAnimationList.emplace_back(scene, i, this);
			}

			m_pAnimator = std::make_shared<Animator>(m_vAnimationList.data(), m_boneCounter);

			size_t matBufferSize = m_boneCounter * sizeof(glm::mat4);
			m_pAnimDataMatSSBO = std::make_shared<GLShaderStorageBuffer>(matBufferSize);
			size_t dqBufferSize = m_boneCounter * sizeof(glm::mat2x4);
			m_pAnimDataDualQuatSSBO = std::make_shared<GLShaderStorageBuffer>(dqBufferSize);
		}

		m_material = std::make_unique<Material>(vertPath, fragPath);
		m_material->resetTextures(m_modelData.pCustom->texturesLoaded);
		m_color = glm::vec3(1.0f, 0.0f, 1.0f);
		m_basicMaterial = m_material->getBasicMaterial();
		m_basicMaterial->ambient = glm::vec3(0.09f, 0.09f, 0.09f);
		m_basicMaterial->diffuse = m_color;
		m_basicMaterial->specular = glm::vec3(0.5f, 0.5f, 0.5f);
		m_basicMaterial->shininess = 0.1f;

		m_scale = glm::vec3(1.0f);
		m_translation = glm::vec3(0.0f);
		m_rotation = glm::vec3(0.0f);
	}

	GLObject::~GLObject() = default;

	void GLObject::bind() const
	{
		m_material->bind();
		if (m_modelDataType == ModelDataType::RAW)
		{
			m_modelData.pRaw->VAO->bind();
			if (m_modelData.pRaw->IBO)
				m_modelData.pRaw->IBO->bind();
		}
	}

	void GLObject::unbind() const
	{
		m_material->unbind();
		if (m_modelDataType == ModelDataType::RAW)
		{
			m_modelData.pRaw->VAO->unbind();
			if (m_modelData.pRaw->IBO)
				m_modelData.pRaw->IBO->unbind();
		}
	}

	void GLObject::onRender(const Renderer& renderer)
	{
		// 这里可能要改
		if (m_isVisible)
		{
			this->bind();
			if (m_modelDataType == ModelDataType::RAW)
			{
				if (m_modelData.pRaw->IBO)
				{
					renderer.draw(*m_modelData.pRaw->VAO, *m_modelData.pRaw->IBO, m_material->getShader());
				}
				else
				{
					// LOG_DEBUG(std::format("[GLObject] draw without ibo, pointsCount = {}.", this->getVBOSize() / this->getVBOLayout()->getStride()));
					renderer.draw(*m_modelData.pRaw->VAO, this->getVBOSize() / this->getVBOLayout()->getStride(), m_material->getShader());
				}
			}
			if (m_modelDataType == ModelDataType::CUSTOM)
			{
				for (auto& mesh : m_modelData.pCustom->meshes)
				{
					mesh.onRender(m_material->getShader(), renderer);
				}
			}
		}
	}

	void GLObject::onImGuiRender(const std::string& ObjectName)
	{
		const std::string objID("##" + std::to_string(m_uuid()));
		if (ImGui::CollapsingHeader((ObjectName + objID).c_str()))
		{
			ImGui::SeparatorText(std::string("Transforms" + objID).c_str());
			ImGui::DragFloat3(std::string("Scale" + objID).c_str(), &m_scale.x, 0.005f, 0.0f, 10.0f);
			ImGui::DragFloat3(std::string("Rotate(Euler Angle)" + objID).c_str(), &m_rotation.x, 0.25f, -360.0f, 360.0f);
			ImGui::DragFloat3(std::string("Translate" + objID).c_str(), &m_translation.x, 0.25f, -100.0f, 100.0f);

			if (m_material->isTexturesEmpty())
			{
				ImGui::SeparatorText(std::string("Material" + objID).c_str());
				ImGui::DragFloat3(std::string("Ambient" + objID).c_str(), &m_basicMaterial->ambient.r, 0.005f, 0.0f, 1.0f);
				ImGui::DragFloat3(std::string("Diffuse" + objID).c_str(), &m_basicMaterial->diffuse.r, 0.005f, 0.0f, 1.0f);
				ImGui::DragFloat3(std::string("Specular" + objID).c_str(), &m_basicMaterial->specular.r, 0.005f, 0.0f, 1.0f);
				ImGui::DragFloat(std::string("Shininess" + objID).c_str(), &m_basicMaterial->shininess, 0.005f, 0.0f, 1.0f);
			}

			// 人物动画相关
			ImGui::SeparatorText(std::format("Animation##{}", m_uuid()).c_str());
			ImGui::Checkbox(std::string(std::format("Enable Animation##{}", m_uuid())).c_str(), &m_isEnableAnimation);
			if (m_isEnableAnimation)
			{
				ImGui::Checkbox(std::format("Enable Simple LERP Blend##{}", m_uuid()).c_str(),
								&m_isEnableLerpBlending);
				if (m_isEnableLerpBlending)
				{
					ImGui::SliderFloat(std::format("BlendFactor##{}", m_uuid()).c_str(),
									   &m_lerpBlendingFactor, 0.0f, 1.0f, "%.1f");
				}

				ImGui::Checkbox(std::format("Enable CrossFading Blend##{}", m_uuid()).c_str(),
								&m_isEnableCrossFadeBlending);
				ImGui::Checkbox(std::format("Enable Partial Blend##{}", m_uuid()).c_str(),
								&m_isEnablePartialBlending);
				ImGui::Checkbox(std::format("Enable Additive Blend##{}", m_uuid()).c_str(),
								&m_isEnableAdditiveBlending);
				if (m_isEnableCrossFadeBlending || m_isEnablePartialBlending)
				{
					if (ImGui::BeginCombo(std::format("Src Clip##{}", m_uuid()).c_str(),
										  m_vAnimationList[m_srcAnimationIdx].GetName().c_str()))
					{
						for (int idx = 0; idx < m_vAnimationList.size(); ++idx)
						{
							std::string name = m_vAnimationList[idx].GetName();
							const bool isSelected = (m_srcAnimationIdx == idx);
							if (ImGui::Selectable(std::format("{}##src{}", name, m_uuid()).c_str(),
												  isSelected))
							{
								m_srcAnimationIdx = idx;
							}
							if (isSelected)
							{
								ImGui::SetItemDefaultFocus();
							}
						}
						ImGui::EndCombo();
					}
					if (ImGui::BeginCombo(std::format("Dst Clip##{}", m_uuid()).c_str(),
										  m_vAnimationList[m_dstAnimationIdx].GetName().c_str()))
					{
						for (int idx = 0; idx < m_vAnimationList.size(); ++idx)
						{
							std::string name = m_vAnimationList[idx].GetName();
							const bool isSelected = (m_dstAnimationIdx == idx);
							if (ImGui::Selectable(std::format("{}##dst{}", name, m_uuid()).c_str(),
												  isSelected))
							{
								m_dstAnimationIdx = idx;
							}
							if (isSelected)
							{
								ImGui::SetItemDefaultFocus();
							}
						}
						ImGui::EndCombo();
					}
					if (m_pAnimator == nullptr || m_pAnimator->GetDstAnimIsNullptr() ||
						m_pAnimator->GetCurAnimationName() != m_vAnimationList[m_srcAnimationIdx].GetName() ||
						m_pAnimator->GetDstAnimationName() != m_vAnimationList[m_dstAnimationIdx].GetName())
					{
						//LOG_DEBUG(std::format("[{}]: create Animator [{}] & [{}]", __FUNCTION__,
						//					  m_vAnimationList[m_srcAnimationIdx].GetName(),
						//					  m_vAnimationList[m_dstAnimationIdx].GetName()));
						m_pAnimator = std::make_shared<Animator>(&m_vAnimationList[m_srcAnimationIdx],
																 &m_vAnimationList[m_dstAnimationIdx],
																 m_boneCounter);
					}
				}
				else
				{
					if (ImGui::BeginCombo(std::format("Clips##{}", m_uuid()).c_str(),
										  m_vAnimationList[m_currentAnimationIdx].GetName().c_str()))
					{
						for (int idx = 0; idx < m_vAnimationList.size(); ++idx)
						{
							std::string name = m_vAnimationList[idx].GetName();

							const bool isSelected = (m_currentAnimationIdx == idx);
							if (ImGui::Selectable(std::format("{}##{}", name, m_uuid()).c_str(),
												  isSelected))
							{
								m_currentAnimationIdx = idx;
							}
							if (isSelected)
							{
								ImGui::SetItemDefaultFocus();
							}
						}
						ImGui::EndCombo();
					}

					// TODO: 做一个Add to queue的按钮, 将选中动画添加到播放序列中, 然后整合Animator的两个构造函数
					if (m_pAnimator == nullptr ||
						m_pAnimator->GetCurAnimationName() != m_vAnimationList[m_currentAnimationIdx].GetName())
					{
						m_pAnimator = std::make_shared<Animator>(&m_vAnimationList[m_currentAnimationIdx], m_boneCounter);
					}
				}

				ImGui::SliderFloat(std::format("Speed##{}", m_uuid()).c_str(),
								   &m_playSpeed, -2.0f, 2.0f, "%.1f");
				ImGui::SliderFloat(std::format("Duration##{}", m_uuid()).c_str(),
								   m_pAnimator->GetCurClipTimeRef(), 0.0f, m_pAnimator->GetCurClipDuration());
				if (ImGui::Button(std::format("Print Bones##{}", m_uuid()).c_str()))
				{
					m_pAnimator->PrintBoneHierarchy();
				}

				m_pAnimator->SetEnableSimpleLerpBlending(m_isEnableLerpBlending);
				m_pAnimator->SetPoseClipBlendFactor(m_lerpBlendingFactor);
				m_pAnimator->SetEnableCrossFadeBlending(m_isEnableCrossFadeBlending);
				m_pAnimator->SetEnablePartialBlend(m_isEnablePartialBlending);
				m_pAnimator->SetEnableAdditiveBlend(m_isEnableAdditiveBlending);

				// TODO: 硬编码, 移动到毕设记得删掉
				//m_pAnimator->SetSrcClipForAdditiveBlend(&m_vAnimationList[24]);
				//m_pAnimator->SetRefClipForAdditiveBlend(&m_vAnimationList[23]);
			}

			ImGui::SeparatorText(std::string("Attributes" + objID).c_str());
			ImGui::Checkbox(std::string("Visibility" + objID).c_str(), &m_isVisible);
			ImGui::Text(std::string("ID: " + std::to_string(m_uuid())).c_str());
		}
	}

	void GLObject::onUpdate(float dt)
	{
		// 渲染帧
		if (getDataType() == GLCore::ModelDataType::RAW)
		{
			setUniform("u_Material.diffuse", 0);
			setUniform("u_Material.specular", 1);
			setUniform("u_Material.shininess", getBasicMaterial()->shininess);
		}
		else
		{
			setUniform("u_HasTextures", !getModelData()->pCustom->texturesLoaded.empty());
			setUniform("u_Material.ambient", getBasicMaterial()->ambient);
			setUniform("u_Material.diffuse", getBasicMaterial()->diffuse);
			setUniform("u_Material.specular", getBasicMaterial()->specular);
			setUniform("u_Material.shininess", getBasicMaterial()->shininess);
		}

		setUniform("u_enableAnimation", m_isEnableAnimation);
		if (m_isEnableAnimation)
		{
			m_pAnimator->UpdateAnimation(dt * m_playSpeed);

			setUniform("u_useDualQuat", m_pAnimator->GetUseDualQuaternion());
			setUniform("u_boneCount", m_boneCounter);
			auto matTransforms = m_pAnimator->GetFinalBoneMatrices();
			m_pAnimDataMatSSBO->writeSsboData<glm::mat4>(matTransforms, 1);
			auto dqTransforms = m_pAnimator->GetFinalBoneDualQuaternions();
			m_pAnimDataDualQuatSSBO->writeSsboData<glm::mat2x4>(dqTransforms, 2);
		}
	}

	void GLObject::processNode(aiNode* node, const aiScene* scene)
	{
		// 处理当前节点所有的aiMesh
		for (unsigned int i = 0; i < node->mNumMeshes; ++i)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			m_modelData.pCustom->meshes.push_back(processMesh(mesh, scene));
		}
		// 递归处理子节点
		for (unsigned int i = 0; i < node->mNumChildren; ++i)
		{
			processNode(node->mChildren[i], scene);
		}
	}

	Mesh GLObject::processMesh(aiMesh* mesh, const aiScene* scene)
	{
		std::vector<MeshVertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<unsigned int> textures;

		// 处理顶点信息
		for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
		{
			MeshVertex vertex;

			glm::vec3 tmpVec3 = {mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z};
			vertex.Position = tmpVec3;

			if (mesh->HasNormals())
			{
				tmpVec3 = {mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z};
				vertex.Normal = tmpVec3;
			}

			if (mesh->mTextureCoords[0])
			{
				glm::vec2 tmpVec2 = {mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y};
				vertex.TexCoords = tmpVec2;
			}

			// 骨骼动画数据初始化
			InitVertexBoneData(vertex);

			vertices.push_back(vertex);
		}

		// 处理骨骼动画数据
		if (mesh->HasBones())
		{
			ExtractBoneWeightForVertices(vertices, mesh, scene);
		}
		else
		{
			LOG_DEBUG(std::format("[{}]: This mesh has no bones for skinning animation.", __FUNCTION__));
		}

		// 处理索引信息
		for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
		{
			aiFace face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; ++j)
			{
				indices.push_back(face.mIndices[j]);
			}
		}

		// 处理材质信息
		if (mesh->mMaterialIndex >= 0)
		{
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			std::vector<unsigned int> diffuseMap = loadCustomTextures(material, aiTextureType_DIFFUSE);
			if (!diffuseMap.empty())
				textures.insert(std::end(textures), std::begin(diffuseMap), std::end(diffuseMap));

			std::vector<unsigned int> specularMap = loadCustomTextures(material, aiTextureType_SPECULAR);
			if (!specularMap.empty())
				textures.insert(std::end(textures), std::begin(specularMap), std::end(specularMap));

			std::vector<unsigned int> reflectionMap = loadCustomTextures(material, aiTextureType_AMBIENT);
			if (!reflectionMap.empty())
				textures.insert(std::end(textures), std::begin(reflectionMap), std::end(reflectionMap));
		}

		return {vertices, indices, textures, &m_modelData.pCustom->texturesLoaded};
	}

	std::vector<unsigned int> GLObject::loadCustomTextures(aiMaterial* material, aiTextureType type) const
	{
		std::vector<unsigned int> textures;
		for (unsigned int i = 0; i < material->GetTextureCount(type); ++i)
		{
			aiString str;
			material->GetTexture(type, i, &str);

			bool skipLoad = false;
			for (unsigned int j = 0; j < m_modelData.pCustom->texturesLoaded.size(); ++j)
			{
				std::string fileName = m_modelData.pCustom->texturesLoaded.at(j)->getFilePath();
				fileName = fileName.substr(fileName.find_last_of('\\') + 1);
				if (std::strcmp(fileName.c_str(), str.C_Str()) == 0)
				{
					textures.push_back(j);
					skipLoad = true;
					break;
				}
			}
			if (!skipLoad)
			{
				TextureType tmpType;
				switch (type)
				{
					case aiTextureType_DIFFUSE:
						tmpType = TextureType::DiffuseMap;
						break;
					case aiTextureType_SPECULAR:
						tmpType = TextureType::SpecularMap;
						break;
					case aiTextureType_AMBIENT:
						tmpType = TextureType::AmbientMap;
						break;
					default:
						tmpType = TextureType::Unknown;
						LOG_ERROR(std::format("[GLObject] loadCustomTextures() read an unknown type texture({})!", static_cast<int>(type)));
						break;
				}
				m_modelData.pCustom->texturesLoaded.push_back(std::make_shared<GLTexture>(m_modelData.pCustom->modelDir + "\\" + std::string(str.C_Str()), tmpType));
				// LOG_INFO(std::format("LOAD PATH: {}", m_modelData.pCustom->modelDir + "\\" + std::string(str.C_Str())));
				textures.push_back(m_modelData.pCustom->texturesLoaded.size() - 1);
			}
		}
		return textures;
	}

	void GLObject::InitVertexBoneData(MeshVertex& vertex)
	{
		for (int i = 0; i < MAX_BONE_INFLUENCE; ++i)
		{
			vertex.m_BoneIDs[i] = -1;
			vertex.m_BoneWeights[i] = 0.0f;
		}
	}

	void GLObject::SetVertexBoneData(MeshVertex& vertex, int boneID, float weight)
	{
		for (int i = 0; i < MAX_BONE_INFLUENCE; ++i)
		{
			if (vertex.m_BoneIDs[i] < 0)
			{
				vertex.m_BoneIDs[i] = boneID;
				vertex.m_BoneWeights[i] = weight;
				break;
			}
		}
	}

	void GLObject::ExtractBoneWeightForVertices(std::vector<MeshVertex>& vertices, aiMesh* mesh, const aiScene* scene)
	{
		for (int boneIdx = 0; boneIdx < mesh->mNumBones; ++boneIdx)
		{
			int boneID = -1;
			std::string boneName = mesh->mBones[boneIdx]->mName.C_Str();

			if (!m_boneInfoMap.contains(boneName))
			{
				BoneInfo newBoneInfo;
				newBoneInfo.id = m_boneCounter++;
				newBoneInfo.offset = AssimpGLMHelper::GetGLMMat4(mesh->mBones[boneIdx]->mOffsetMatrix);
				m_boneInfoMap[boneName] = newBoneInfo;

				boneID = newBoneInfo.id;
			}
			else
			{
				boneID = m_boneInfoMap[boneName].id;
			}

			if (boneID == -1)
			{
				LOG_ERROR(std::format("[{}]: Cannot get boneID!", __FUNCTION__));
				__debugbreak();
			}

			auto weights = mesh->mBones[boneIdx]->mWeights;
			int numWeights = mesh->mBones[boneIdx]->mNumWeights;
			for (int weightsIdx = 0; weightsIdx < numWeights; ++weightsIdx)
			{
				int vertexID = weights[weightsIdx].mVertexId;
				float weight = weights[weightsIdx].mWeight;

				if (vertexID > vertices.size())
				{
					LOG_ERROR(std::format("[{}]: vertices array out of bound!", __FUNCTION__));
					__debugbreak();
				}

				SetVertexBoneData(vertices[vertexID], boneID, weight);
			}
		}
	}

	void GLObject::resetTextures(const std::initializer_list<TextureDesc>& list) const
	{
		if (m_material)
		{
			m_material->resetTextures(list);
		}
		else
		{
			LOG_WARN("[GLObject] resetTextures() when m_material == nullptr !!!");
		}
	}

	void GLObject::resetTextures(const std::vector<std::shared_ptr<GLTexture>>& textures) const
	{
		if (m_material)
		{
			m_material->resetTextures(textures);
		}
		else
		{
			LOG_WARN("[GLObject] resetTextures() when m_material == nullptr !!!");
		}
	}
}
