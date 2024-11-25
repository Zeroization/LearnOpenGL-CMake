#include "Animation/Animator.h"
#include "Animation/AnimBlendHelper.hpp"


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

	Animator::Animator(Animation* pSrcAnimation, Animation* pDstAnimation,
					   int boneCount)
	{
		m_currentAnimation = pSrcAnimation;
		m_pDstAnimation = pDstAnimation;

		if ((m_blendOpts & AnimBlendOption::CrossFading) != AnimBlendOption::None)
		{
			m_enableBlendingForCrossFading = true;
		}
		if ((m_blendOpts & AnimBlendOption::PartialSkeleton) != AnimBlendOption::None)
		{
			m_enableBlendingForPartial = true;
		}

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

		if (m_currentAnimation && m_pDstAnimation && m_enableBlendingForCrossFading)
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
						m_boneDualQuaternions[i] = AnimBlendHelper::LERPDuelQuaterion(srcTransDq[i],
																					  m_boneDualQuaternions[i],
																					  m_blendFactorForCrossFading);
					}
				}
				else
				{
					for (size_t i = 0; i < srcTransMat.size(); ++i)
					{
						m_finalBoneMatrices[i] = AnimBlendHelper::LERPMat4(srcTransMat[i],
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
		else if (m_currentAnimation && m_pDstAnimation && m_enableBlendingForPartial)
		{
			for (size_t i = 0; i < m_srcAnimMaskNameList.size(); ++i)
			{
				m_currentAnimation->SetAnimMaskHierarchy(m_srcAnimMaskNameList.at(i),
														 m_currentAnimation->GetRootNode(),
														 false);
			}

			m_pDstAnimation->SetAnimMaskHierarchy("bone_root",
												  m_pDstAnimation->GetRootNode(),
												  false);
			for (size_t i = 0; i < m_srcAnimMaskNameList.size(); ++i)
			{
				m_pDstAnimation->SetAnimMaskHierarchy(m_srcAnimMaskNameList.at(i),
													  m_pDstAnimation->GetRootNode(),
													  true);
			}

			float curAnimDuration = m_currentAnimation->GetDuration();
			float dstAnimDuration = m_pDstAnimation->GetDuration();

			m_currentTime += (m_currentAnimation->GetTicksPerSecond() + m_pDstAnimation->GetTicksPerSecond()) * 0.5 * dt;
			m_currentTime = fmod(m_currentTime, curAnimDuration);

			CalculateBoneTransform(m_currentAnimation, &m_currentAnimation->GetRootNode(),
								   m_currentTime, glm::mat4(1.0f));
			std::vector<glm::mat4> srcTransMat(m_finalBoneMatrices);
			std::vector<glm::mat2x4> srcTransDq(m_boneDualQuaternions);
			CalculateBoneTransform(m_pDstAnimation, &m_pDstAnimation->GetRootNode(),
								   fmod(m_currentTime, dstAnimDuration), glm::mat4(1.0f));

			if (m_useDualQuaternion)
			{
				for (size_t i = 0; i < srcTransDq.size(); ++i)
				{
					float partialFactor = m_currentAnimation->GetAnimMaskById(i) ? 1.0 : 0.0;

					m_boneDualQuaternions[i] = AnimBlendHelper::LERPDuelQuaterion(srcTransDq[i], 
																				  m_boneDualQuaternions[i], 
																				  partialFactor);
				}
			}
			else
			{
				for (size_t i = 0; i < srcTransMat.size(); ++i)
				{
					float partialFactor = m_currentAnimation->GetAnimMaskById(i) ? 1.0 : 0.0;
					
					m_finalBoneMatrices[i] = AnimBlendHelper::LERPMat4(srcTransMat[i],
																	   m_finalBoneMatrices[i],
																	   partialFactor);
				}
			}
		}
		else if (m_currentAnimation && m_pSrcClip && m_pRefClip && m_enableBlendingForAdditive)
		{
			float curClipDuration = m_currentAnimation->GetDuration();

			m_currentTime += m_currentAnimation->GetTicksPerSecond() * dt;
			m_currentTime = fmod(m_currentTime, curClipDuration);

			// 1. 先求差分序列
			float diffClipDuration = std::min(m_pSrcClip->GetDuration(), m_pRefClip->GetDuration());

			CalculateBoneTransform(m_pSrcClip, &m_pSrcClip->GetRootNode(),
								   fmod(m_currentTime, diffClipDuration), glm::mat4(1.0f));
			std::vector<glm::mat4> srcTransMat(m_finalBoneMatrices);
			std::vector<glm::mat2x4> srcTransDq(m_boneDualQuaternions);

			CalculateBoneTransform(m_pRefClip, &m_pRefClip->GetRootNode(),
								   fmod(m_currentTime, diffClipDuration), glm::mat4(1.0f));
			std::vector<glm::mat4> refTransMat(m_finalBoneMatrices);
			std::vector<glm::mat2x4> refTransDq(m_boneDualQuaternions);

			// 2. 再求原序列
			CalculateBoneTransform(m_currentAnimation, &m_currentAnimation->GetRootNode(),
								   m_currentTime, glm::mat4(1.0f));

			// 3. 进行叠加
			if (m_useDualQuaternion)
			{
				for (size_t i = 0; i < m_boneDualQuaternions.size(); ++i)
				{
					m_boneDualQuaternions[i] = srcTransDq[i] - refTransDq[i] + m_boneDualQuaternions[i];
				}
			}
			else
			{
				for (size_t i = 0; i < m_finalBoneMatrices.size(); ++i)
				{
					m_finalBoneMatrices[i] = (srcTransMat[i] * glm::inverse(refTransMat[i])) * m_finalBoneMatrices[i];
				}
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

		std::string nodeName = pNode->name;
		glm::mat4 nodeTransform = pNode->transformation;

		Bone* bone = pAnimation->FindBone(nodeName);

		if (bone)
		{
			bone->Update(curTime);

			if (m_enableBlendingForPoseClip)
			{
				nodeTransform = AnimBlendHelper::LERPMat4(pNode->transformation, bone->GetLocalTransform(),
														  m_blendFactorForPoseClip);
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

			// 临时变量
			glm::quat orientation;
			glm::vec3 scale;
			glm::vec3 translation;
			glm::vec3 skew;
			glm::vec4 perspective;

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
