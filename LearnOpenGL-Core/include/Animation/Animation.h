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
		int childrenCount = 0;
		std::vector<AssimpNodeData> children;

		AssimpNodeData() = default;
	};

	class Animation
	{
	public:
		Animation() = default;
		Animation(const aiScene* scene, unsigned int animIdx, GLObject* model);

		Bone* FindBone(const std::string& name);

		inline float GetTicksPerSecond() const { return m_ticksPerSecond; }
		inline float GetDuration() const { return m_duration; }
		inline std::string GetName() const { return m_name; }
		inline const AssimpNodeData& GetRootNode() { return m_rootNode; }
		inline const std::map<std::string, BoneInfo>& GetBoneIDMap() { return m_boneInfoMap; }

	private:
		void ReadMissingBones(const aiAnimation* animation, GLObject& model);
		void ReadHeirarchyData(AssimpNodeData& dest, const aiNode* src);

	private:
		float m_duration;
		float m_ticksPerSecond;
		std::string m_name;

		AssimpNodeData m_rootNode;
		std::unordered_map<std::string, Bone> m_name2Bones;
		std::map<std::string, BoneInfo> m_boneInfoMap;
	};
}
