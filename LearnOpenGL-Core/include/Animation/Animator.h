#pragma once

#include "Animation/Animation.h"

namespace GLCore
{
	class Animator
	{
	public:
		Animator(Animation* animation);

		void UpdateAnimation(float dt);
		void PlayAnimation(Animation* pAnimation);

		void CalculateBoneTransform(const AssimpNodeData* node, glm::mat4 parentTransform);

		std::vector<glm::mat4> GetFinalBoneMatrices() { return m_finalBoneMatrices; }
		std::string GetCurAnimationName() const { return m_currentAnimation->GetName(); }

	private:
		std::vector<glm::mat4> m_finalBoneMatrices;
		Animation* m_currentAnimation;
		float m_currentTime;
		float m_deltaTime;
	};
}
