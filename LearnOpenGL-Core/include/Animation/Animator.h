#pragma once

#include "Animation/Animation.h"

namespace GLCore
{
	class Animator
	{
	public:
		Animator(Animation* animation, int boneCount);

		void UpdateAnimation(float dt);
		void PlayAnimation(Animation* pAnimation);

		void CalculateBoneTransform(const AssimpNodeData* node, glm::mat4 parentTransform);

		bool GetUseDualQuaternion() const { return m_useDualQuaternion; }
		float* GetCurClipTimeRef() { return &m_currentTime; }
		float GetCurClipDuration() const { return m_currentAnimation->GetDuration(); }
		std::vector<glm::mat4> GetFinalBoneMatrices() { return m_finalBoneMatrices; }
		std::vector<glm::mat2x4> GetFinalBoneDualQuaternions() { return m_boneDualQuaternions; }
		std::string GetCurAnimationName() const { return m_currentAnimation->GetName(); }

	private:
		Animation* m_currentAnimation;
		float m_currentTime;
		float m_deltaTime;
		bool m_useDualQuaternion = true;
		std::vector<glm::mat4> m_finalBoneMatrices;
		std::vector<glm::mat2x4> m_boneDualQuaternions;
	};
}
