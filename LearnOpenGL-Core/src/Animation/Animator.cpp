#include "Animation/Animator.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/dual_quaternion.hpp"
#include "glm/gtx/matrix_decompose.hpp"

namespace GLCore
{
	Animator::Animator(Animation* animation, int boneCount)
	{
		m_deltaTime = 0.0f;
		m_currentTime = 0.0f;
		m_currentAnimation = animation;

		m_finalBoneMatrices = std::vector<glm::mat4>(boneCount, glm::mat4(1.0f));

		glm::dualquat dq;
		dq[0] = glm::quat(1, 0, 0, 0);
		dq[1] = glm::quat(0, 0, 0, 0);
		m_boneDualQuaternions = std::vector<glm::mat2x4>(boneCount, glm::mat2x4_cast(dq));
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

			// 使用对偶四元数
			//	1. 准备临时变量
			glm::quat orientation;
			glm::vec3 scale;
			glm::vec3 translation;
			glm::vec3 skew;
			glm::vec4 perspective;
			//	2. 拆解蒙皮矩阵
			if (glm::decompose(m_finalBoneMatrices[index], scale, orientation, translation, skew, perspective))
			{
				glm::dualquat dq;
				dq[0] = orientation;
				dq[1] = glm::quat(0.0, translation.x, translation.y, translation.z) * orientation * 0.5f;
				m_boneDualQuaternions[index] = glm::mat2x4_cast(dq);
			}
			else
			{
				m_useDualQuaternion = false;
			}
		}

		for (int i = 0; i < node->childrenCount; ++i)
			CalculateBoneTransform(&node->children[i], globalTransform);
	}
}
