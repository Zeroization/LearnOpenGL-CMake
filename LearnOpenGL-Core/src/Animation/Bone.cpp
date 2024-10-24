#include "Animation/Bone.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/quaternion.hpp"

namespace GLCore
{
	Bone::Bone(const std::string& name, int ID, const aiNodeAnim* channel)
		: m_ID(ID), m_name(name), m_localTransform(glm::mat4(1.0f))
	{
		m_numTranslates = channel->mNumPositionKeys;
		for (int posIdx = 0; posIdx < m_numTranslates; ++posIdx)
		{
			aiVector3D aiPosition = channel->mPositionKeys[posIdx].mValue;
			float timeStamp = channel->mPositionKeys[posIdx].mTime;

			m_keyTranslations.push_back({AssimpGLMHelpers::GetGLMVec3(aiPosition), timeStamp});
		}

		m_numRotates = channel->mNumRotationKeys;
		for (int rotIdx = 0; rotIdx < m_numRotates; ++rotIdx)
		{
			aiQuaternion aiRotation = channel->mRotationKeys[rotIdx].mValue;
			float timeStamp = channel->mRotationKeys[rotIdx].mTime;

			m_keyRotations.push_back({AssimpGLMHelpers::GetGLMQuat(aiRotation), timeStamp});
		}

		m_numScales = channel->mNumScalingKeys;
		for (int scaleIdx = 0; scaleIdx < m_numScales; ++scaleIdx)
		{
			aiVector3D aiScaling = channel->mScalingKeys[scaleIdx].mValue;
			float timeStamp = channel->mScalingKeys[scaleIdx].mTime;

			m_keyScales.push_back({AssimpGLMHelpers::GetGLMVec3(aiScaling), timeStamp});
		}
	}

	void Bone::Update(float animationTime)
	{
		glm::mat4 translate = InterpolateTranslate(animationTime);
		glm::mat4 rotate = InterpolateRotate(animationTime);
		glm::mat4 scale = InterpolateScale(animationTime);
		m_localTransform = translate * rotate * scale;
	}

	int Bone::GetTranslateIndex(float animationTime) const
	{
		for (int idx = 0; idx < m_numTranslates - 1; ++idx)
		{
			if (animationTime < m_keyTranslations[idx + 1].timeStamp)
				return idx;
		}

		LOG_ERROR(std::format("[{}]: Invalid animationTime: {}!", __FUNCTION__, animationTime));
		__debugbreak();
		return -1;
	}

	int Bone::GetRotateIndex(float animationTime) const
	{
		for (int idx = 0; idx < m_numRotates - 1; ++idx)
		{
			if (animationTime < m_keyRotations[idx + 1].timeStamp)
				return idx;
		}

		LOG_ERROR(std::format("[{}]: Invalid animationTime: {}!", __FUNCTION__, animationTime));
		__debugbreak();
		return -1;
	}

	int Bone::GetScaleIndex(float animationTime) const
	{
		for (int idx = 0; idx < m_numScales - 1; ++idx)
		{
			if (animationTime < m_keyScales[idx + 1].timeStamp)
				return idx;
		}

		LOG_ERROR(std::format("[{}]: Invalid animationTime: {}!", __FUNCTION__, animationTime));
		__debugbreak();
		return -1;
	}

	float Bone::GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime) const
	{
		float scaleFactor = 0.0f;
		float midWayLength = animationTime - lastTimeStamp;
		float framesDiff = nextTimeStamp - lastTimeStamp;
		scaleFactor = midWayLength / framesDiff;
		return scaleFactor;
	}

	glm::mat4 Bone::InterpolateTranslate(float animationTime)
	{
		if (m_numTranslates == 1)
			return glm::translate(glm::mat4(1.0f), m_keyTranslations[0].translate);

		int p0Idx = GetTranslateIndex(animationTime);
		int p1Idx = p0Idx + 1;
		float scaleFactor = GetScaleFactor(m_keyTranslations[p0Idx].timeStamp,
										   m_keyTranslations[p1Idx].timeStamp,
										   animationTime);
		glm::vec3 finalTranslate = glm::mix(m_keyTranslations[p0Idx].translate,
											m_keyTranslations[p1Idx].translate,
											scaleFactor);
		return glm::translate(glm::mat4(1.0f), finalTranslate);
	}

	glm::mat4 Bone::InterpolateRotate(float animationTime)
	{
		if (m_numRotates == 1)
		{
			auto rotate = glm::normalize(m_keyRotations[0].orientation);
			return glm::toMat4(rotate);
		}

		int p0Idx = GetRotateIndex(animationTime);
		int p1Idx = p0Idx + 1;
		float scaleFactor = GetScaleFactor(m_keyRotations[p0Idx].timeStamp,
										   m_keyRotations[p1Idx].timeStamp,
										   animationTime);
		glm::quat finalRotate = glm::slerp(m_keyRotations[p0Idx].orientation,
										   m_keyRotations[p1Idx].orientation,
										   scaleFactor);
		finalRotate = glm::normalize(finalRotate);
		return glm::toMat4(finalRotate);
	}

	glm::mat4 Bone::InterpolateScale(float animationTime)
	{
		if (m_numRotates == 1)
			return glm::scale(glm::mat4(1.0f), m_keyScales[0].scale);

		int p0Idx = GetScaleIndex(animationTime);
		int p1Idx = p0Idx + 1;
		float scaleFactor = GetScaleFactor(m_keyScales[p0Idx].timeStamp,
										   m_keyScales[p1Idx].timeStamp,
										   animationTime);
		glm::vec3 finalScale = glm::mix(m_keyScales[p0Idx].scale,
										m_keyScales[p1Idx].scale,
										scaleFactor);
		return glm::scale(glm::mat4(1.0f), finalScale);
	}
}
