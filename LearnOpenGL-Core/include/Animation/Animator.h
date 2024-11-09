#pragma once

#include "Animation/Animation.h"

namespace GLCore
{
	class Animator
	{
	public:
		// 只播放单独一个动画
		Animator(Animation* animation, int boneCount);

		// 先播放srcAnimation, 再CrossFading到dstAnimation
		Animator(Animation* pSrcAnimation, Animation* pDstAnimation, int boneCount);

		void UpdateAnimation(float dt);

		void CalculateBoneTransform(Animation* pAnimation, const AssimpNodeData* pNode,
									float curTime, glm::mat4 parentTransform);

		void SetEnableSimpleLerpBlending(bool val) { m_enableBlendingForPoseClip = val; }
		void SetEnableCrossFadeBlending(bool val) { m_enableBlendingForCrossFading = val; }
		void SetPoseClipBlendFactor(float val) { m_blendFactorForPoseClip = val; }
		bool GetUseDualQuaternion() const { return m_useDualQuaternion; }
		bool GetDstAnimIsNullptr() const { return m_pDstAnimation == nullptr; }
		float* GetCurClipTimeRef() { return &m_currentTime; }
		float GetCurClipDuration() const {
			return m_enableBlendingForCrossFading ?
				m_currentAnimation->GetDuration() + m_pDstAnimation->GetDuration() :
				m_currentAnimation->GetDuration();
		}
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

		// 动画混合 - 绑定姿势和当前Clip简单线性混合
		bool m_enableBlendingForPoseClip = false;
		float m_blendFactorForPoseClip = 1.0f;

		// 动画混合 - CrossFading
		bool m_enableBlendingForCrossFading = false;
		float m_blendFactorForCrossFading = 1.0f;
		Animation* m_pDstAnimation = nullptr;
	};
}
