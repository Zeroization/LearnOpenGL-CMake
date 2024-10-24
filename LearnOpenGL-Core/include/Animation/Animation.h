#pragma once

#include "Core/Object.h"

#include "Animation/Bone.h"

namespace GLCore
{
	// 用于和Assimp库解耦的节点类
	struct AssimpNodeData
	{
		glm::mat4 transformation;
		std::string name;
		int childrenCount;
		std::vector<AssimpNodeData> children;
	};

	class Animation
	{
	public:
		Animation() = default;
		Animation(const std::string& animationPath, GLObject* model);

		Bone* FindBone(const std::string& name);

		inline float GetTicksPerSecond() const { return m_ticksPerSecond; }
		inline float GetDuration() const { return m_duration; }
		inline const AssimpNodeData& GetRootNode() { return m_rootNode; }
		inline const std::map<std::string, BoneInfo>& GetBoneIDMap() { return m_boneInfoMap; }

	private:
		void ReadMissingBones(const aiAnimation* animation, GLObject& model);
		void ReadHeirarchyData(AssimpNodeData& dest, const aiNode* src);

	private:
		float m_duration;
		float m_ticksPerSecond;
		std::unordered_map<std::string, Bone> m_name2Bones;
		AssimpNodeData m_rootNode;
		std::map<std::string, BoneInfo> m_boneInfoMap;
	};
}
