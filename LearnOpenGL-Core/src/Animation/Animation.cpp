#include "Animation/Animation.h"
#include "Animation/Bone.h"
#include "Utils/AssimpGLMHelper.hpp"

#include "assimp/postprocess.h"

namespace GLCore
{
	Animation::Animation(const std::string& animationPath, GLObject* model)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(animationPath, aiProcess_Triangulate);
		assert(scene && scene->mRootNode);

		auto animation = scene->mAnimations[0];
		m_duration = animation->mDuration;
		m_ticksPerSecond = animation->mTicksPerSecond;

		ReadHeirarchyData(m_rootNode, scene->mRootNode);
		ReadMissingBones(animation, *model);
	}

	Bone* Animation::FindBone(const std::string& name)
	{
		if (m_name2Bones.contains(name))
			return &m_name2Bones[name];
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

	void Animation::ReadHeirarchyData(AssimpNodeData& dest, const aiNode* src)
	{
		assert(src);

		dest.name = src->mName.data;
		dest.transformation = AssimpGLMHelpers::GetGLMMat4(src->mTransformation);
		dest.childrenCount = src->mNumChildren;

		for (int i = 0; i < src->mNumChildren; ++i)
		{
			AssimpNodeData newData;
			ReadHeirarchyData(newData, src->mChildren[i]);
			dest.children.push_back(newData);
		}
	}
}
