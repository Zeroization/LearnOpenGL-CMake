#include "Animation/Animator.h"

namespace GLCore
{
	Animator::Animator(Animation* animation)
	{
		m_deltaTime = 0.0f;
		m_currentTime = 0.0f;
		m_currentAnimation = animation;
		m_finalBoneMatrices = std::vector<glm::mat4>(100, glm::mat4(1.0f));
	}

	void Animator::UpdateAnimation(float dt)
	{
		m_deltaTime = dt;
		if (m_currentAnimation)
		{
			m_currentTime += m_currentAnimation->GetTicksPerSecond() * dt;
			m_currentTime = fmod(m_currentTime, m_currentAnimation->GetDuration());
			CalculateBoneTransform(&m_currentAnimation->GetRootNode(), glm::mat4(1.0f));
		}
	}

	void Animator::PlayAnimation(Animation* pAnimation)
	{
		m_currentAnimation = pAnimation;
		m_currentTime = 0.0f;
	}

	void Animator::CalculateBoneTransform(const AssimpNodeData* node, glm::mat4 parentTransform)
	{
		std::string nodeName = node->name;
		glm::mat4 nodeTransform = node->transformation;

		Bone* bone = m_currentAnimation->FindBone(nodeName);

		if (bone)
		{
			bone->Update(m_currentTime);
			nodeTransform = bone->GetLocalTransform();
		}

		glm::mat4 globalTransform = parentTransform * nodeTransform;

		auto boneInfoMap = m_currentAnimation->GetBoneIDMap();
		if (boneInfoMap.contains(nodeName))
		{
			int index = boneInfoMap[nodeName].id;
			glm::mat4 offset = boneInfoMap[nodeName].offset;
			m_finalBoneMatrices[index] = globalTransform * offset;
		}

		for (int i = 0; i < node->childrenCount; ++i)
			CalculateBoneTransform(&node->children[i], globalTransform);
	}
}
