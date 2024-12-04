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
				CalculateBoneTransform(m_currentAnimation, m_currentAnimation->GetRootNode(),
									   m_currentTime, glm::mat4(1.0f));
				std::vector<glm::mat4> srcTransMat(m_finalBoneMatrices);
				std::vector<glm::mat2x4> srcTransDq(m_boneDualQuaternions);
				CalculateBoneTransform(m_pDstAnimation, m_pDstAnimation->GetRootNode(),
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
				CalculateBoneTransform(m_currentAnimation, m_currentAnimation->GetRootNode(),
									   m_currentTime, glm::mat4(1.0f));
			}
			else if (m_currentTime > endTime)
			{
				CalculateBoneTransform(m_pDstAnimation, m_pDstAnimation->GetRootNode(),
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

			CalculateBoneTransform(m_currentAnimation, m_currentAnimation->GetRootNode(),
								   m_currentTime, glm::mat4(1.0f));
			std::vector<glm::mat4> srcTransMat(m_finalBoneMatrices);
			std::vector<glm::mat2x4> srcTransDq(m_boneDualQuaternions);
			CalculateBoneTransform(m_pDstAnimation, m_pDstAnimation->GetRootNode(),
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

			CalculateBoneTransform(m_pSrcClip, m_pSrcClip->GetRootNode(),
								   fmod(m_currentTime, diffClipDuration), glm::mat4(1.0f));
			std::vector<glm::mat4> srcTransMat(m_finalBoneMatrices);
			std::vector<glm::mat2x4> srcTransDq(m_boneDualQuaternions);

			CalculateBoneTransform(m_pRefClip, m_pRefClip->GetRootNode(),
								   fmod(m_currentTime, diffClipDuration), glm::mat4(1.0f));
			std::vector<glm::mat4> refTransMat(m_finalBoneMatrices);
			std::vector<glm::mat2x4> refTransDq(m_boneDualQuaternions);

			// 2. 再求原序列
			CalculateBoneTransform(m_currentAnimation, m_currentAnimation->GetRootNode(),
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
			CalculateBoneTransform(m_currentAnimation, m_currentAnimation->GetRootNode(),
								   m_currentTime, glm::mat4(1.0f));
		}

		// 计算IK
		if (m_animIKOpt == 1 && CheckIkParam())
		{
			m_isCalcIk = true;

			// 准备IK需要的参数
			auto boneIDMap = m_currentAnimation->GetBoneIDMap();
			Bone* pRootBone = m_currentAnimation->FindBone(m_ikChainParam.vpNodeList[2]->name);
			BoneInfo rootBoneInfo = boneIDMap[m_ikChainParam.vpNodeList[2]->name];
			glm::quat rootGlobalRotation = AnimBlendHelper::GetRotationFromMat4(m_finalBoneMatrices[rootBoneInfo.id] * 
																				rootBoneInfo.offset);
			glm::quat rootLocalRotation = pRootBone->GetCurOrientation();
			
			Bone* pMiddleBone = m_currentAnimation->FindBone(m_ikChainParam.vpNodeList[1]->name);
			BoneInfo middleBoneInfo = boneIDMap[m_ikChainParam.vpNodeList[1]->name];
			glm::quat middleGlobalRotation = AnimBlendHelper::GetRotationFromMat4(m_finalBoneMatrices[middleBoneInfo.id] *
																				  middleBoneInfo.offset);
			glm::quat middleLocalRotation = pMiddleBone->GetCurOrientation();

			// 进行IK
			AnimTwoBoneIKSolver::SolveTwoBoneIK(m_ikChainParam.vNodeWorldPosList[2], m_ikChainParam.vNodeWorldPosList[1],
												m_ikChainParam.vNodeWorldPosList[0], m_ikChainParam.targetWorldPos,
												rootGlobalRotation, middleGlobalRotation,
												rootLocalRotation, middleLocalRotation);

			// 结束IK, 计算骨骼
			pRootBone->SetCurOrientation(rootLocalRotation);
			pRootBone->SetLocalTransformByCurSRT();
			pMiddleBone->SetCurOrientation(middleLocalRotation);
			pMiddleBone->SetLocalTransformByCurSRT();

			glm::mat4 parentTransform = glm::mat4(1.0f);
			if (m_ikChainParam.vpNodeList[2]->pParentNode)
			{
				BoneInfo parentBoneInfo = boneIDMap[m_ikChainParam.vpNodeList[2]->pParentNode->name];
				parentTransform = m_finalBoneMatrices[parentBoneInfo.id] * glm::inverse(parentBoneInfo.offset);
			}

			CalculateBoneTransform(m_currentAnimation, m_ikChainParam.vpNodeList[2], m_currentTime, parentTransform);

			m_isCalcIk = false;
		}
		else if (m_animIKOpt == 2 && CheckIkParam())
		{
			m_isCalcIk = true;

			// 准备IK需要的参数
			auto boneIDMap = m_currentAnimation->GetBoneIDMap();
			std::vector<glm::quat> vNodeGlobalRotation;
			for (size_t i = 0; i < m_ikChainParam.vpNodeList.size(); ++i)
			{
				AssimpNodeData* pCurNode = m_ikChainParam.vpNodeList.at(i);
				Bone* pCurBone = m_currentAnimation->FindBone(pCurNode->name);
				BoneInfo curBoneInfo = boneIDMap[pCurNode->name];

				glm::quat globalRotation = AnimBlendHelper::GetRotationFromMat4(m_finalBoneMatrices[curBoneInfo.id] *
																				curBoneInfo.offset);
				glm::quat localRotation = pCurBone->GetCurOrientation();
				vNodeGlobalRotation.emplace_back(globalRotation);
			}

			// 进行IK
			// TODO: 看看能不能封装到SolveCCDIK函数里, 估计不行了...
			const float CCD_THRESHOLD = 0.00001f;
			for (int i = 0; i < m_ikIterationCnt; ++i)
			{
				// 符合条件就提前结束
				if (glm::length(m_ikChainParam.targetWorldPos - m_ikChainParam.vNodeWorldPosList.at(0)) <= CCD_THRESHOLD)
				{
					break;
				}

				// 从Effector的下一个节点开始遍历
				for (int j = 1; j < m_ikChainParam.vpNodeList.size(); ++j)
				{
					AssimpNodeData* pCurNode = m_ikChainParam.vpNodeList.at(j);
					Bone* pCurBone = m_currentAnimation->FindBone(pCurNode->name);
					BoneInfo curBoneInfo = boneIDMap[pCurNode->name];

					glm::vec3 curWorldPos = m_ikChainParam.vNodeWorldPosList.at(j);
					glm::quat curGlobalRotation = vNodeGlobalRotation.at(j);

					// 计算Global坐标系下的旋转变换量
					glm::vec3 toEffector = glm::normalize(m_ikChainParam.vNodeWorldPosList.at(0) - curWorldPos);
					glm::vec3 toTarget = glm::normalize(m_ikChainParam.targetWorldPos - curWorldPos);
					glm::quat effectorToTarget = glm::normalize(glm::rotation(toEffector, toTarget));

					// 获取Local坐标系下的旋转变换量
					glm::quat localRotation = curGlobalRotation * effectorToTarget * glm::conjugate(curGlobalRotation);
					localRotation = glm::normalize(localRotation);

					// 在Local坐标系下旋转当前节点
					glm::quat currentRotation = glm::normalize(pCurBone->GetCurOrientation() * localRotation);
					pCurBone->SetCurOrientation(currentRotation);
					pCurBone->SetLocalTransformByCurSRT();

					// 更新矩阵
					glm::mat4 parentTransform = glm::mat4(1.0f);
					AssimpNodeData* pParentNode = pCurNode->pParentNode;
					if (pParentNode != nullptr)
					{
						BoneInfo rootParentBoneInfo = boneIDMap[pParentNode->name];
						parentTransform = m_finalBoneMatrices[rootParentBoneInfo.id] * glm::inverse(rootParentBoneInfo.offset);
					}
					CalculateBoneTransform(m_currentAnimation, pCurNode, m_currentTime, parentTransform);

					// 更新世界坐标
					for (int k = j - 1; k >= 0; --k)
					{
						AssimpNodeData* pTmpNode = m_ikChainParam.vpNodeList.at(k);
						BoneInfo tmpBoneInfo = boneIDMap[pTmpNode->name];
						glm::mat4 tmpModelMat = m_ikChainParam.objModelMat *
							m_finalBoneMatrices[tmpBoneInfo.id] * glm::inverse(tmpBoneInfo.offset);
						m_ikChainParam.vNodeWorldPosList.at(k) = tmpModelMat[3];
					}

					// 符合条件就提前结束
					if (glm::length(m_ikChainParam.targetWorldPos - m_ikChainParam.vNodeWorldPosList.at(0)) <= CCD_THRESHOLD)
					{
						break;
					}
				}
			}

			m_isCalcIk = false;
		}
	}

	void Animator::CalculateBoneTransform(Animation* pAnimation, AssimpNodeData* pNode, float curTime, glm::mat4 parentTransform)
	{
		std::string nodeName = pNode->name;
		glm::mat4 nodeTransform = pNode->transformation;

		Bone* bone = pAnimation->FindBone(nodeName);

		if (bone)
		{
			if (!m_isCalcIk)
			{
				bone->Update(curTime);
			}

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
			CalculateBoneTransform(pAnimation, pNode->pChildren[i].get(), curTime, globalTransform);
	}

	bool Animator::CheckIkParam()
	{
		if (m_ikChainParam.vpNodeList.empty() || m_ikChainParam.vNodeWorldPosList.empty())
		{
			return false;
		}

		for (size_t i = 0; i < m_ikChainParam.vpNodeList.size(); ++i)
		{
			if (!m_ikChainParam.vpNodeList.at(i))
			{
				return false;
			}
		}
	
		return true;
	}
}
