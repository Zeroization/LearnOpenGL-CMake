#include "Animation/Animation.h"

#include "Core/Object.h"
#include "Utils/AssimpGLMHelper.hpp"

#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"

#include <algorithm>
#include <queue>


namespace GLCore
{
	Animation::Animation(const aiScene* scene, unsigned int animIdx, GLObject* model)
	{
		auto animation = scene->mAnimations[animIdx];
		m_duration = animation->mDuration;
		m_ticksPerSecond = animation->mTicksPerSecond;
		m_name = animation->mName.C_Str();

		ReadHierarchyData(&m_rootNode, scene->mRootNode);
		ReadMissingBones(animation, *model);

		m_animMask.resize(m_boneInfoMap.size());
		std::fill(m_animMask.begin(), m_animMask.end(), true);
	}

	Bone* Animation::FindBone(const std::string& name)
	{
		if (m_name2Bones.contains(name))
			return &m_name2Bones[name];
		return nullptr;
	}

	void Animation::SetAnimMaskHierarchy(const std::string& boneName, const AssimpNodeData& rootNode, bool value)
	{
		if (rootNode.name == boneName)
		{
			SetAnimMaskHierarchy(rootNode, value);
		}
		else
		{
			for (int i = 0; i < rootNode.childrenCount; ++i)
			{
				SetAnimMaskHierarchy(boneName, *rootNode.pChildren[i], value);
			}
		}
	}


	void Animation::SetAnimMaskHierarchy(const AssimpNodeData& node, bool value)
	{
		if (m_boneInfoMap.contains(node.name))
		{
			int id = m_boneInfoMap[node.name].id;
			m_animMask.at(id) = value;
		}
		for (int i = 0; i < node.childrenCount; ++i)
		{
			SetAnimMaskHierarchy(*node.pChildren[i], value);
		}
	}

	void Animation::PrintBoneHierarchy(const AssimpNodeData& node, int level)
	{
		std::string tab;
		for (int i = 0; i < level; ++i)
			tab += " ";
		tab += "-";

		if (m_boneInfoMap.contains(node.name))
			LOG_INFO(tab + node.name);
		for (int i = 0; i < node.childrenCount; ++i)
		{
			PrintBoneHierarchy(*node.pChildren[i], level + 1);
		}
	}

	AssimpNodeData* Animation::GetAssimpNodeByBoneName(const std::string& boneName)
	{
		if (FindBone(boneName))
		{
			// 层序遍历, 懒得递归了
			std::queue<AssimpNodeData*> queue;
			queue.push(&m_rootNode);

			while (!queue.empty())
			{
				AssimpNodeData* pNode = queue.front();
				queue.pop();

				if (pNode->name == boneName)
					return pNode;
				
				for (size_t i = 0; i < pNode->childrenCount; ++i)
				{
					queue.push(pNode->pChildren[i].get());
				}
			}
		}

		return nullptr;
	}

	void Animation::ReadMissingBones(const aiAnimation* animation, GLObject& model)
	{
		int size = animation->mNumChannels;
		
		auto& boneInfoMap = model.getBoneInfoMap();
		int& boneCount = model.getBoneCount();

		for (int i = 0; i < size; ++i)
		{
			auto channel = animation->mChannels[i];
			std::string boneName = channel->mNodeName.data;

			if (!boneInfoMap.contains(boneName))
			{
				boneInfoMap[boneName].id = boneCount;
				boneCount++;
			}
			m_name2Bones[boneName] = Bone(channel->mNodeName.data, boneInfoMap[channel->mNodeName.data].id, channel);
		}

		m_boneInfoMap = boneInfoMap;
	}

	void Animation::ReadHierarchyData(AssimpNodeData* dest, const aiNode* src)
	{
		assert(dest && src);

		dest->name = src->mName.data;
		dest->transformation = AssimpGLMHelper::GetGLMMat4(src->mTransformation);
		dest->childrenCount = src->mNumChildren;

		dest->pChildren.clear();
		for (int i = 0; i < src->mNumChildren; ++i)
		{
			auto child = std::make_unique<AssimpNodeData>();
			child->pParentNode = dest;
			ReadHierarchyData(child.get(), src->mChildren[i]);
			dest->pChildren.emplace_back(std::move(child));
		}
	}
}
