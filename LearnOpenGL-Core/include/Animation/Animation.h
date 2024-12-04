#pragma once

#include "Animation/Bone.h"

#include "assimp/scene.h"


namespace GLCore
{
	class GLObject;

	// 用于和Assimp库解耦的节点类
	struct AssimpNodeData
	{
		glm::mat4 transformation = glm::mat4(1.0f);
		std::string name;

		AssimpNodeData* pParentNode = nullptr;
		int childrenCount = 0;
		std::vector<std::unique_ptr<AssimpNodeData>> pChildren;

		AssimpNodeData() = default;

		// 禁止拷贝
		AssimpNodeData(const AssimpNodeData&) = delete;
		AssimpNodeData& operator=(const AssimpNodeData&) = delete;

		// 允许移动
		AssimpNodeData(AssimpNodeData&&) noexcept = default;
		AssimpNodeData& operator=(AssimpNodeData&&) noexcept = default;
	};

	class Animation
	{
	public:
		Animation() = default;
		Animation(const aiScene* scene, unsigned int animIdx, GLObject* model);

		Bone* FindBone(const std::string& name);

		void SetAnimMaskHierarchy(const std::string& boneName, AssimpNodeData* rootNode, bool value);

		void PrintBoneHierarchy(AssimpNodeData* rootNode, int level = 0);

		AssimpNodeData* GetAssimpNodeByBoneName(const std::string& boneName);

		inline GLObject* GetModelRef() const { return m_pModelRef; }
		inline float GetTicksPerSecond() const { return m_ticksPerSecond; }
		inline float GetDuration() const { return m_duration; }
		inline void SetName(const std::string& name) { m_name = name; }
		inline std::string GetName() const { return m_name; }
		inline AssimpNodeData* GetRootNode() { return m_pRootNode.get(); }
		inline const std::map<std::string, BoneInfo>& GetBoneIDMap() { return m_boneInfoMap; }
		inline const std::vector<bool>& GetAnimMask() { return m_animMask; }
		inline const bool GetAnimMaskById(int id) { return id >= m_animMask.size() ? true : m_animMask[id]; }

	private:
		void SetAnimMaskHierarchy(AssimpNodeData* rootNode, bool value);

		void ReadMissingBones(const aiAnimation* animation, GLObject& model);
		void ReadHierarchyData(AssimpNodeData* dest, const aiNode* src);

	private:
		GLObject* m_pModelRef = nullptr;
		float m_duration;
		float m_ticksPerSecond;
		std::string m_name;

		// 骨骼信息
		std::unordered_map<std::string, Bone> m_name2Bones;
		std::map<std::string, BoneInfo> m_boneInfoMap;
		std::vector<bool> m_animMask;

		std::unique_ptr<AssimpNodeData> m_pRootNode;
	};
}
