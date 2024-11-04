#pragma once

#include "pch.hpp"
#include "assimp/anim.h"
#include "Utils/AssimpGLMHelper.hpp"

#include <vector>
#include <string>

namespace GLCore
{
	// ======================= 关键帧定义 =======================
	struct KeyTranslate
	{
		glm::vec3 translate;
		float timeStamp;

		KeyTranslate(const glm::vec3& _translate, float _timeStamp)
			: translate(_translate), timeStamp(_timeStamp) {}
	};

	struct KeyRotate
	{
		glm::quat orientation;
		float timeStamp;

		KeyRotate(const glm::quat& _orientation, float _timeStamp)
			: orientation(_orientation), timeStamp(_timeStamp) {}
	};

	struct KeyScale
	{
		glm::vec3 scale;
		float timeStamp;

		KeyScale(const glm::vec3& _scale, float _timeStamp)
			: scale(_scale), timeStamp(_timeStamp) {}
	};
	// ======================= 关键帧定义 =======================

	// 骨骼动画 - 关节数据
	struct BoneInfo
	{
		int id;
		glm::mat4 offset;	// 将顶点从模型空间变换到关节局部空间的矩阵
	};

	class Bone
	{
	public:
		Bone() = default;
		// 从aiNodeAnim->channel中读取该骨骼/关节的关键帧数据
		Bone(const std::string& name, int ID, const aiNodeAnim* channel);

		// 根据动画的当前时间给SRT变换插值，并更新该骨骼/关节的局部空间M矩阵
		void Update(float animationTime);

		// ======================= Getters & Setters =======================
		// 根据给定播放时间获取最近位置的关键平移帧索引
		int GetTranslateIndex(float animationTime) const;
		// 根据给定播放时间获取最近位置的关键旋转帧索引
		int GetRotateIndex(float animationTime) const;
		// 根据给定播放时间获取最近位置的关键缩放帧索引
		int GetScaleIndex(float animationTime) const;
		glm::vec3 GetCurTranslate() const { return m_curTranslate; }
		glm::vec3 GetCurScale() const { return m_curScale; }
		glm::quat GetCurOrientation() const { return m_curOrientation; }
		std::string GetBoneName() const { return m_name; }
		glm::mat4 GetLocalTransform() const { return m_localTransform; }
		// ======================= Getters & Setters =======================

	private:
		// 获取用于Lerp和SLerp的标准化值
		float GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime) const;
		// 使用简单线性插值获取当前播放时间的平移矩阵
		glm::mat4 InterpolateTranslate(float animationTime);
		// 使用简单线性插值获取当前播放时间的旋转矩阵
		glm::mat4 InterpolateRotate(float animationTime);
		// 使用简单线性插值获取当前播放时间的缩放矩阵
		glm::mat4 InterpolateScale(float animationTime);

	private:
		int m_ID = -1;
		std::string m_name;

		int m_numTranslates = 0;
		int m_numRotates = 0;
		int m_numScales = 0;
		std::vector<KeyTranslate> m_keyTranslations;
		std::vector<KeyRotate> m_keyRotations;
		std::vector<KeyScale> m_keyScales;

		glm::vec3 m_curTranslate;
		glm::vec3 m_curScale;
		glm::quat m_curOrientation;
		glm::mat4 m_localTransform = glm::mat4(1.0f);
	};
}