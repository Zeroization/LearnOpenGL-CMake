﻿#pragma once

#include "pch.hpp"

#include "Core/Renderer.h"
#include "Core/Material.h"
#include "Core/UUID.h"
#include "Geometry/Mesh.h"
#include "Animation/Animator.h"
#include "OpenGL/glShaderStorageBuffer.h"

#include "assimp/scene.h"

#include <map>

namespace GLCore
{
	enum class ModelDataType
	{
		RAW = 0,
		CUSTOM
	};

	// 适用于简单模型的数据结构(如正方体等)
	struct RawModelData
	{
		float* verticesCache = nullptr;
		size_t verticesSize = 0;
		unsigned* indicesCache = nullptr;
		int indicesCount = 0;

		std::unique_ptr<GLVertexBuffer> VBO;
		std::unique_ptr<GLVertexBufferLayout> VBLayout;
		std::unique_ptr<GLVertexArray> VAO;
		std::unique_ptr<GLIndexBuffer> IBO;
	};

	// 适用于复杂模型的数据结构(如自定义模型)
	struct CustomModelData
	{
		std::vector<Mesh> meshes;
		std::string modelDir;
		std::vector<std::shared_ptr<GLTexture>> texturesLoaded;
	};

	struct ModelData
	{
		std::unique_ptr<RawModelData> pRaw;
		std::unique_ptr<CustomModelData> pCustom;
	};


	class GLObject
	{
	public:
		// vv--------------------------- RawModel ----------------------------vv
		GLObject(float vertices[], size_t verticesSize, const GLVertexBufferLayout& vertLayout,
				 const std::string& vertPath, const std::string& fragPath,
				 const std::vector<TextureDesc>& textureDescList = {});
		GLObject(float vertices[], size_t verticesSize, const GLVertexBufferLayout& vertLayout,
				 unsigned int* indices, int iCount,
				 const std::string& vertPath, const std::string& fragPath,
				 const std::vector<TextureDesc>& textureDescList = {});
		// ^^--------------------------- RawModel ----------------------------^^

		// vv ----------------------- CustomModel ------------------------vv
		explicit GLObject(const std::string& modelPath,
		                  const std::string& vertPath = std::string(PROJ_RES_PATH) + "/Shaders/CustomModel/model.vert",
		                  const std::string& fragPath = std::string(PROJ_RES_PATH) + "/Shaders/CustomModel/model.frag");
		// ^^ ----------------------- CustomModel ------------------------^^

		virtual ~GLObject();

		virtual void bind() const;
		virtual void unbind() const;

		virtual void onRender(const Renderer& renderer);
		virtual void onImGuiRender(const std::string& ObjectName);
		virtual void onUpdate(float dt);

		inline bool isVisible() const { return m_isVisible; }
		inline void setVisibility(bool isVisible) { m_isVisible = isVisible; }
		inline glm::vec3 getColor() const { return m_color; }
		inline void setColor(float r, float g, float b) { m_color = glm::normalize(glm::vec3(r, g, b)); }
		inline BasicMaterial* getBasicMaterial() const { return m_basicMaterial; }
		inline std::string getUUID() const { return std::to_string(m_uuid()); }
		inline ModelDataType getDataType() const { return m_modelDataType; }
		inline const ModelData* getModelData() const { return &m_modelData; }
		inline const Material* getMaterial() const { return  m_material.get(); }
		inline const glm::vec3& getScale() const { return m_scale; }
		inline void setScale(float x, float y, float z) { m_scale = glm::vec3(x, y, z); }
		inline void setScale(const glm::vec3& scale) { m_scale = scale; }
		inline const glm::vec3& getRotation() const { return m_rotation; }
		inline void setRotation(float x, float y, float z) { m_rotation = glm::vec3(x, y, z); }
		inline void setRotation(const glm::vec3& rotation) { m_rotation = rotation; }
		inline const glm::vec3& getTranslation() const { return m_translation; }
		inline void setTranslation(float x, float y, float z) { m_translation = glm::vec3(x, y, z); }
		inline void setTranslation(const glm::vec3& translation) { m_translation = translation; }
		inline glm::mat4 getModelMat() const
		{
			glm::mat4 scale = glm::scale(glm::mat4(1.0f), m_scale);
			glm::mat4 rotate = glm::mat4_cast(glm::quat(radians(m_rotation)));
			glm::mat4 translate = glm::translate(glm::mat4(1.0f), m_translation);

			return translate * rotate * scale;
		}
		template <typename T>
		void setUniform(const std::string& name, const T& val)
		{
			if (m_material && !m_material->isShaderEmpty())
			{
				m_material->setUniform(name, val);
			}
			else
			{
				LOG_ERROR("[{0}] {1}", "Object", "Material or Shader is nullptr!");
			}
		}

		void resetTextures(const std::initializer_list<TextureDesc>& list) const;
		void resetTextures(const std::vector<std::shared_ptr<GLTexture>>& textures) const;


		// vv--------------------------- CustomModel -------------------------vv
		void processNode(aiNode* node, const aiScene* scene);
		Mesh processMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<unsigned int> loadCustomTextures(aiMaterial* material, aiTextureType type) const;

		void InitVertexBoneData(MeshVertex& vertex);
		void ExtractBoneWeightForVertices(std::vector<MeshVertex>& vertices, aiMesh* mesh, const aiScene* scene);

		void SetVertexBoneData(MeshVertex& vertex, int boneID, float weight);
		void SetEnableAnimation(bool val) { m_isEnableAnimation = val; }
		void SetEnableAnimLerpBlend(bool val) { m_isEnableLerpBlending = val; }
		void SetEnableAnimCrossfadeBlend(bool val) { m_isEnableCrossFadeBlending = val; }
		void SetEnableAnimPartialBlend(bool val) { m_isEnablePartialBlending = val; }
		void SetAnimMaskJointNameList(const std::vector<std::string>& jointNames) { m_vJointNamesForAnimMask = jointNames; }
		void SetEnableAnimAdditiveBlend(bool val) { m_isEnableAdditiveBlending = val; }
		void SetAnimSrcRefClipForAdditiveBlend(const std::string& srcClipName, const std::string& refClipName);
		void SetAnimIKOpt(int animIkOpt) { m_pAnimator->SetAnimIkOpt(animIkOpt); }
		void SetIkIterCnt(int iterCnt) { m_pAnimator->SetIkIterCnt(iterCnt); }
		void SetIkChainParams(const std::string& effectorBoneName, const glm::vec3& targetWorldPos, int ikChainNodeSize = 4);
		IkChainParams GetIkChainParams() const { return m_pAnimator->GetIkChainParam(); }
		auto& getBoneInfoMap() { return m_boneInfoMap; }
		int& getBoneCount() { return m_boneCounter; }
		glm::mat4 GetBoneMatByName(const std::string& name) 
		{ 
			// 别忘了将globalTransform * offset (Local Space)还原为globalTransform (Model Space)
			return m_boneInfoMap.contains(name) ?
				m_pAnimator->GetFinalBoneMatrices().at(m_boneInfoMap[name].id) * glm::inverse(m_boneInfoMap[name].offset) :
				glm::mat4(1.0f);
		}
		// ^^--------------------------- CustomModel -------------------------^^

		// vv--------------------------- RawModel ----------------------------vv
		inline GLIndexBuffer* getIBO() const { return m_modelDataType == ModelDataType::RAW ? m_modelData.pRaw->IBO.get() : nullptr; }
		inline size_t getVBOSize() const { return m_modelDataType == ModelDataType::RAW ? m_modelData.pRaw->verticesSize : -1; }
		inline GLVertexBufferLayout* getVBOLayout() const { return m_modelDataType == ModelDataType::RAW ? m_modelData.pRaw->VBLayout.get() : nullptr; }
		inline void addTexture(GLCore::GLTexture* texPtr) const { m_material->addTexture(texPtr); }
		// ^^--------------------------- RawModel ----------------------------^^

	protected:
		// 对象的数据
		ModelData m_modelData;
		ModelDataType m_modelDataType;

		// 对象本体属性
		UUID m_uuid;
		bool m_isVisible;

		// 对象变换属性
		glm::vec3 m_scale;
		glm::vec3 m_rotation;
		glm::vec3 m_translation;

		// 对象材质属性
		glm::vec3 m_color;
		std::unique_ptr<Material> m_material;
		BasicMaterial* m_basicMaterial;

		// 对象骨骼属性
		std::map<std::string, BoneInfo> m_boneInfoMap;
		int m_boneCounter = 0;

		// 对象动画属性
		bool m_isEnableAnimation = false;
		bool m_isEnableLerpBlending = false;
		bool m_isEnableCrossFadeBlending = false;
		bool m_isEnablePartialBlending = false;
		bool m_isEnableAdditiveBlending = false;

		float m_playSpeed = 1.0f;
		float m_lerpBlendingFactor = 1.0f;
		int m_currentAnimationIdx = 0;
		int m_srcAnimationIdx = 0;
		int m_dstAnimationIdx = 0;

		std::shared_ptr<Animator> m_pAnimator;
		std::vector<Animation> m_vAnimationList;
		std::vector<std::string> m_vJointNamesForAnimMask;
		std::shared_ptr<GLShaderStorageBuffer> m_pAnimDataMatSSBO;
		std::shared_ptr<GLShaderStorageBuffer> m_pAnimDataDualQuatSSBO;
	};
}