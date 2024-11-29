#pragma once

#include "Animation/Animation.h"

namespace GLCore
{
	class AnimTwoBoneIKSolver
	{
	public:
		static void SolveTwoBoneIK(glm::vec3 rootPos, glm::vec3 middlePos, glm::vec3 effectorPos,
								   glm::vec3 targetPos,
								   glm::quat rootGlobalRotation, glm::quat middleGlobalRotation,
								   glm::quat& rootLocalRotation, glm::quat& middleLocalRotation,
								   float eps = 0.0001f)
		{
			glm::vec3 ab = middlePos - rootPos;
			glm::vec3 ac = effectorPos - rootPos;
			glm::vec3 at = targetPos - rootPos;
			glm::vec3 cb = middlePos - effectorPos;

			// Step1: 旋转关节root和middle, 让dist(root, effector) == dist(root, target)
			float len_ab = glm::length(ab);
			float len_cb = glm::length(cb);
			// 计算可达性
			float len_at = glm::clamp(glm::length(at), eps, len_ab + len_cb + eps);
			// 计算Step1中, 关节root和middle的旋转角
			float angle_ac_ab_0 = std::acos(std::clamp(glm::dot(glm::normalize(ac), glm::normalize(ab)),
													   -1.f, 1.f));
			float angle_ba_bc_0 = std::acos(std::clamp(glm::dot(glm::normalize(-ab), glm::normalize(-cb)),
													   -1.f, 1.f));
			float angle_ac_ab_1 = std::acos(std::clamp(static_cast<float>(
				(len_cb * len_cb - len_ab * len_ab - len_at * len_at) / (-2.0 * len_ab * len_at)),
				-1.f, 1.f));
			float angle_ba_bc_1 = std::acos(std::clamp(static_cast<float>(
				(len_at * len_at - len_ab * len_ab - len_cb * len_cb) / (-2.0 * len_ab * len_cb)),
				-1.f, 1.f));
			float angle_ac_ab = angle_ac_ab_1 - angle_ac_ab_0;
			float angle_ba_bc = angle_ba_bc_1 - angle_ba_bc_0;
			// 计算Step1中, 关节root和middle的旋转轴
			glm::vec3 d = glm::rotate(middleGlobalRotation, glm::vec3(0, 0, 1));
			glm::vec3 axis0 = glm::normalize(glm::cross(ac, d));
			// 旋转关节root和middle, 注意旋转轴"世界空间->模型本地空间"的转换
			rootLocalRotation = glm::rotate(rootLocalRotation, angle_ac_ab, axis0 * glm::inverse(rootGlobalRotation));
			middleLocalRotation = glm::rotate(middleLocalRotation, angle_ba_bc, axis0 * glm::inverse(rootGlobalRotation));

			// Step2: 旋转root关节, 让Effector到达Target
			float angle_ac_at = std::acos(std::clamp(glm::dot(glm::normalize(ac), glm::normalize(at)),
													 -1.f, 1.f));
			glm::vec3 axis1 = glm::normalize(glm::cross(ac, at));
			rootLocalRotation = glm::rotate(rootLocalRotation, angle_ac_at, axis1 * glm::inverse(rootGlobalRotation));
		}
	};
}
