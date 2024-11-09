#include "Animation/Animator.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/dual_quaternion.hpp"
#include "glm/gtx/matrix_decompose.hpp"
#include "glm/gtx/matrix_interpolation.hpp"
#include "glm/gtx/quaternion.hpp"

namespace GLCore
{
	Animator::Animator(Animation* animation, int boneCount)
	{
		m_currentAnimation = animation;

		m_deltaTime = 0.0f;
		m_currentTime = 0.0f;
		m_finalBoneMatrices = std::vector<glm::mat4>(boneCount, glm::mat4(1.0f));
		glm::dualquat dq;
		dq[0] = glm::quat(1, 0, 0, 0);
		dq[1] = glm::quat(0, 0, 0, 0);
		m_boneDualQuaternions = std::vector<glm::mat2x4>(boneCount, glm::mat2x4_cast(dq));
	}

	Animator::Animator(Animation* pSrcAnimation, Animation* pDstAnimation, int boneCount)
	{
		m_enableBlendingForCrossFading = true;
		m_currentAnimation = pSrcAnimation;
		m_pDstAnimation = pDstAnimation;

		m_deltaTime = 0.0f;
		m_currentTime = 0.0f;
		m_finalBoneMatrices = std::vector<glm::mat4>(boneCount, glm::mat4(1.0f));
		glm::dualquat dq;
		dq[0] = glm::quat(1, 0, 0, 0);
		dq[1] = glm::quat(0, 0, 0, 0);
		m_boneDualQuaternions = std::vector<glm::mat2x4>(boneCount, glm::mat2x4_cast(dq));
	}

	void Animator::UpdateAnimation(float dt)
	{
		m_deltaTime = dt;

		if (m_currentAnimation && m_pDstAnimation)
		{
			float curAnimDuration = m_currentAnimation->GetDuration();
			float dstAnimDuration = m_pDstAnimation->GetDuration();

			m_currentTime += (m_currentAnimation->GetTicksPerSecond() + m_pDstAnimation->GetTicksPerSecond()) * 0.5 * dt;
			m_currentTime = fmod(m_currentTime, curAnimDuration + dstAnimDuration);

			// 看看是否需要CrossFading
			float startTime = curAnimDuration - 0.15 * dstAnimDuration;
			float endTime = curAnimDuration + 0.15 * dstAnimDuration;
			if (startTime <= m_currentTime && m_currentTime <= endTime)
			{
				// 计算混合因子
				float u = (m_currentTime - startTime) / (endTime - startTime);
				float v = 1 - u;
				m_blendFactorForCrossFading = 3 * v * u * u + u * u * u;
				m_blendFactorForCrossFading = std::clamp(m_blendFactorForCrossFading, 0.f, 1.f);

				// CrossFading
				CalculateBoneTransform(m_currentAnimation, &m_currentAnimation->GetRootNode(),
									   m_currentTime, glm::mat4(1.0f));
				std::vector<glm::mat4> srcTransMat(m_finalBoneMatrices);
				std::vector<glm::mat2x4> srcTransDq(m_boneDualQuaternions);
				CalculateBoneTransform(m_pDstAnimation, &m_pDstAnimation->GetRootNode(),
									   fmod(m_currentTime, dstAnimDuration), glm::mat4(1.0f));

				// 混合
				if (m_useDualQuaternion)
				{
					for (size_t i = 0; i < srcTransDq.size(); ++i)
					{
						glm::dualquat srcDq = srcTransDq[i];
						glm::dualquat dstDq = m_boneDualQuaternions[i];
						glm::dualquat resDq = glm::lerp(srcDq, dstDq, m_blendFactorForCrossFading);
						m_boneDualQuaternions[i] = glm::mat2x4_cast(glm::normalize(resDq));
					}
				}
				else
				{
					for (size_t i = 0; i < srcTransMat.size(); ++i)
					{
						m_finalBoneMatrices[i] = glm::interpolate(srcTransMat[i],
																  m_finalBoneMatrices[i],
																  m_blendFactorForCrossFading);
					}
				}
			}
			else if (m_currentTime < startTime)
			{
				CalculateBoneTransform(m_currentAnimation, &m_currentAnimation->GetRootNode(),
									   m_currentTime, glm::mat4(1.0f));
			}
			else if (m_currentTime > endTime)
			{
				CalculateBoneTransform(m_pDstAnimation, &m_pDstAnimation->GetRootNode(),
									   fmod(m_currentTime, dstAnimDuration), 
									   glm::mat4(1.0f));
			}
			
		}
		else if (m_currentAnimation)
		{
			// 一般动画播放
			m_currentTime += m_currentAnimation->GetTicksPerSecond() * dt;
			m_currentTime = fmod(m_currentTime + m_currentAnimation->GetDuration(),
								 m_currentAnimation->GetDuration());
			CalculateBoneTransform(m_currentAnimation, &m_currentAnimation->GetRootNode(),
								   m_currentTime, glm::mat4(1.0f));
		}
	}

	void Animator::CalculateBoneTransform(Animation* pAnimation, const AssimpNodeData* pNode, float curTime, glm::mat4 parentTransform)
	{
		// 临时变量
		glm::quat orientation;
		glm::vec3 scale;
		glm::vec3 translation;
		glm::vec3 skew;
		glm::vec4 perspective;

		std::string nodeName = pNode->name;
		glm::mat4 nodeTransform = pNode->transformation;

		Bone* bone = pAnimation->FindBone(nodeName);

		if (bone)
		{
			bone->Update(curTime);

			if (m_enableBlendingForPoseClip)
			{
				if (glm::decompose(pNode->transformation, scale, orientation, translation, skew, perspective))
				{
					glm::quat interOrientation = glm::slerp(orientation, bone->GetCurOrientation(), m_blendFactorForPoseClip);
					glm::vec3 interTranslate = glm::mix(translation, bone->GetCurTranslate(), m_blendFactorForPoseClip);
					glm::vec3 interScale = glm::mix(scale, bone->GetCurScale(), m_blendFactorForPoseClip);
					nodeTransform = glm::translate(glm::mat4(1.0f), interTranslate) *
						glm::toMat4(interOrientation) *
						glm::scale(glm::mat4(1.0f), interScale);
				}
				else
				{
					nodeTransform = glm::interpolate(pNode->transformation, bone->GetLocalTransform(), 
													 m_blendFactorForPoseClip);
				}
			}
			else
			{
				nodeTransform = bone->GetLocalTransform();
			}
		}

		glm::mat4 globalTransform = parentTransform * nodeTransform;

		auto boneInfoMap = pAnimation->GetBoneIDMap();
		if (boneInfoMap.contains(nodeName))
		{
			int index = boneInfoMap[nodeName].id;
			glm::mat4 offset = boneInfoMap[nodeName].offset;
			m_finalBoneMatrices[index] = globalTransform * offset;

			// 使用对偶四元数
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

		for (int i = 0; i < pNode->childrenCount; ++i)
			CalculateBoneTransform(pAnimation, &pNode->children[i], curTime, globalTransform);
	}
}
