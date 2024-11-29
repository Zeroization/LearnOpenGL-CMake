#pragma once

#include "Animation/Animation.h"
#include "Animation/AnimBlendOption.h"
#include "Animation/AnimTwoBoneIKSolver.hpp"

namespace GLCore
{
	struct TwoBoneIkParams
	{
		AssimpNodeData* pRootNode = nullptr;
		AssimpNodeData* pMiddleNode = nullptr;
		AssimpNodeData* pEffectorNode = nullptr;

		glm::vec3 rootPos;
		glm::vec3 middlePos;
		glm::vec3 effectorPos;
		glm::vec3 targetPos;
	};

	class Animator
	{
	public:
		// 只播放单独一个动画
		Animator(Animation* animation, int boneCount);

		// 先播放srcAnimation, 再CrossFading到dstAnimation
		Animator(Animation* pSrcAnimation, Animation* pDstAnimation, 
				 int boneCount);

		void UpdateAnimation(float dt);

		void CalculateBoneTransform(Animation* pAnimation, AssimpNodeData* pNode,
									float curTime, glm::mat4 parentTransform);

		bool CheckIkParam();

		void PrintBoneHierarchy() const
		{
			m_currentAnimation->PrintBoneHierarchy(m_currentAnimation->GetRootNode());
		}

		void SetEnableSimpleLerpBlending(bool val)
		{
			if (m_enableBlendingForPoseClip == val)
				return;

			m_enableBlendingForPoseClip = val;
			if (val)
				m_blendOpts |= AnimBlendOption::SimpleLERP;
			else
				m_blendOpts -= AnimBlendOption::SimpleLERP;
		}
		void SetEnableCrossFadeBlending(bool val)
		{
			if (m_enableBlendingForCrossFading == val)
				return;

			m_enableBlendingForCrossFading = val;
			if (val)
				m_blendOpts |= AnimBlendOption::CrossFading;
			else
				m_blendOpts -= AnimBlendOption::CrossFading;
		}
		void SetEnablePartialBlend(bool val)
		{
			if (m_enableBlendingForPartial == val)
				return;

			m_enableBlendingForPartial = val;
			if (val)
				m_blendOpts |= AnimBlendOption::PartialSkeleton;
			else
				m_blendOpts -= AnimBlendOption::PartialSkeleton;
		}
		void SetEnableAdditiveBlend(bool val)
		{
			if (m_enableBlendingForAdditive == val)
				return;

			m_enableBlendingForAdditive = val;
			if (val)
				m_blendOpts |= AnimBlendOption::Additive;
			else
				m_blendOpts -= AnimBlendOption::Additive;
		}
		void SetSrcAnimMaskByJointNames(const std::vector<std::string>& jointNames) { m_srcAnimMaskNameList = jointNames; };
		void SetSrcClipForAdditiveBlend(Animation* pSrcClip) { m_pSrcClip = pSrcClip; }
		void SetRefClipForAdditiveBlend(Animation* pRefClip) { m_pRefClip = pRefClip; }
		void SetPoseClipBlendFactor(float val) { m_blendFactorForPoseClip = val; }
		void SetAnimIkOpt(int val) { m_animIKOpt = val; }
		void SetTwoBoneIKParam(const TwoBoneIkParams& params) { m_twoBoneIkParam = params; }
		bool GetUseDualQuaternion() const { return m_useDualQuaternion; }
		bool GetDstAnimIsNullptr() const { return m_pDstAnimation == nullptr; }
		float* GetCurClipTimeRef() { return &m_currentTime; }
		float GetCurClipDuration() const {
			return m_enableBlendingForCrossFading ?
				m_currentAnimation->GetDuration() + m_pDstAnimation->GetDuration() :
				m_currentAnimation->GetDuration();
		}
		Animation* GetCurClip() { return m_currentAnimation; }
		std::vector<glm::mat4> GetFinalBoneMatrices() { return m_finalBoneMatrices; }
		std::vector<glm::mat2x4> GetFinalBoneDualQuaternions() { return m_boneDualQuaternions; }
		std::string GetCurAnimationName() const { return m_currentAnimation->GetName(); }
		std::string GetDstAnimationName() const { return m_pDstAnimation->GetName(); }

	private:
		Animation* m_currentAnimation;
		float m_currentTime;
		float m_deltaTime;
		bool m_useDualQuaternion = true;
		std::vector<glm::mat4> m_finalBoneMatrices;
		std::vector<glm::mat2x4> m_boneDualQuaternions;

		// 动画混合
		AnimBlendOption m_blendOpts;
		// 动画混合 - 绑定姿势和当前Clip简单线性混合
		bool m_enableBlendingForPoseClip = false;
		float m_blendFactorForPoseClip = 1.0f;
		// 动画混合 - CrossFading
		bool m_enableBlendingForCrossFading = false;
		float m_blendFactorForCrossFading = 1.0f;
		Animation* m_pDstAnimation = nullptr;
		// 动画混合 - Partial Blending
		bool m_enableBlendingForPartial = false;
		std::vector<std::string> m_srcAnimMaskNameList;
		// 动画混合 - Additive Blending
		bool m_enableBlendingForAdditive = false;
		Animation* m_pSrcClip = nullptr;
		Animation* m_pRefClip = nullptr;

		// 动画IK
		// 0 - None, 1 - TwoBone, 2 - CCD, 3 - FABRIK
		int m_animIKOpt = 0;
		bool m_isCalcIk = false;
		TwoBoneIkParams m_twoBoneIkParam;

	};
}
