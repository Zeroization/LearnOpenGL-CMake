#include "Animation/Bone.h"

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

			m_keyTranslations.push_back({AssimpGLMHelper::GetGLMVec3(aiPosition), timeStamp});
		}

		m_numRotates = channel->mNumRotationKeys;
		for (int rotIdx = 0; rotIdx < m_numRotates; ++rotIdx)
		{
			aiQuaternion aiRotation = channel->mRotationKeys[rotIdx].mValue;
			float timeStamp = channel->mRotationKeys[rotIdx].mTime;

			m_keyRotations.push_back({AssimpGLMHelper::GetGLMQuat(aiRotation), timeStamp});
		}

		m_numScales = channel->mNumScalingKeys;
		for (int scaleIdx = 0; scaleIdx < m_numScales; ++scaleIdx)
		{
			aiVector3D aiScaling = channel->mScalingKeys[scaleIdx].mValue;
			float timeStamp = channel->mScalingKeys[scaleIdx].mTime;

			m_keyScales.push_back({AssimpGLMHelper::GetGLMVec3(aiScaling), timeStamp});
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
		int idx = 0;
		for (idx = 0; idx < m_numTranslates - 1; ++idx)
		{
			if (animationTime < m_keyTranslations[idx + 1].timeStamp)
				return idx;
		}
		return idx;
	}

	int Bone::GetRotateIndex(float animationTime) const
	{
		int idx = 0;
		for (idx = 0; idx < m_numRotates - 1; ++idx)
		{
			if (animationTime < m_keyRotations[idx + 1].timeStamp)
				return idx;
		}
		return idx;
	}

	int Bone::GetScaleIndex(float animationTime) const
	{
		int idx = 0;
		for (idx = 0; idx < m_numScales - 1; ++idx)
		{
			if (animationTime < m_keyScales[idx + 1].timeStamp)
				return idx;
		}
		return idx;
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
		if (p1Idx == m_numTranslates)
			return glm::translate(glm::mat4(1.0f), m_keyTranslations[p0Idx].translate);

		float scaleFactor = GetScaleFactor(m_keyTranslations[p0Idx].timeStamp,
										   m_keyTranslations[p1Idx].timeStamp,
										   animationTime);
		m_curTranslate = glm::mix(m_keyTranslations[p0Idx].translate,
											m_keyTranslations[p1Idx].translate,
											scaleFactor);
		return glm::translate(glm::mat4(1.0f), m_curTranslate);
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
		if (p1Idx == m_numRotates)
			return glm::toMat4(m_keyRotations[p0Idx].orientation);

		float scaleFactor = GetScaleFactor(m_keyRotations[p0Idx].timeStamp,
										   m_keyRotations[p1Idx].timeStamp,
										   animationTime);
		m_curOrientation = glm::slerp(m_keyRotations[p0Idx].orientation,
										   m_keyRotations[p1Idx].orientation,
										   scaleFactor);
		m_curOrientation = glm::normalize(m_curOrientation);
		return glm::toMat4(m_curOrientation);
	}

	glm::mat4 Bone::InterpolateScale(float animationTime)
	{
		if (m_numRotates == 1)
			return glm::scale(glm::mat4(1.0f), m_keyScales[0].scale);

		int p0Idx = GetScaleIndex(animationTime);
		int p1Idx = p0Idx + 1;
		if (p1Idx == m_numScales)
			return glm::scale(glm::mat4(1.0f), m_keyScales[p0Idx].scale);

		float scaleFactor = GetScaleFactor(m_keyScales[p0Idx].timeStamp,
										   m_keyScales[p1Idx].timeStamp,
										   animationTime);
		m_curScale = glm::mix(m_keyScales[p0Idx].scale,
										m_keyScales[p1Idx].scale,
										scaleFactor);
		return glm::scale(glm::mat4(1.0f), m_curScale);
	}
}
