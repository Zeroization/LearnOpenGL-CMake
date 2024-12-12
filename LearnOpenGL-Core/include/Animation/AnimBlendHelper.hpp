#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/dual_quaternion.hpp"
#include "glm/gtx/matrix_decompose.hpp"
#include "glm/gtx/matrix_interpolation.hpp"
#include "glm/gtx/quaternion.hpp"

namespace GLCore
{
	class AnimBlendHelper
	{
	public:
		static glm::mat2x4 LERPDuelQuaterion(const glm::mat2x4& srcDq, const glm::mat2x4& dstDq, float blendFactor)
		{
			glm::dualquat _srcDq = srcDq;
			glm::dualquat _dstDq = dstDq;
			return glm::mat2x4_cast(LERPDuelQuaterion(_srcDq, _dstDq, blendFactor));
		}

		static glm::dualquat LERPDuelQuaterion(const glm::dualquat& srcDq, const glm::dualquat& dstDq, float blendFactor)
		{
			glm::dualquat _resDq = glm::lerp(srcDq, dstDq, blendFactor);
			return glm::normalize(_resDq);
		}

		static glm::mat4 LERPMat4(const glm::mat4& src, const glm::mat4& dst, float blendFactor)
		{
			glm::quat srcOrientation, dstOrientation;
			glm::vec3 srcScale, dstScale;
			glm::vec3 srcTranslation, dstTranslation;
			glm::vec3 srcSkew, dstSkew;
			glm::vec4 srcPerspective, dstPerspective;

			if (glm::decompose(src, srcScale, srcOrientation, srcTranslation, srcSkew, srcPerspective) &&
				glm::decompose(dst, dstScale, dstOrientation, dstTranslation, dstSkew, dstPerspective))
			{
				glm::quat interOrientation = glm::slerp(srcOrientation, dstOrientation, blendFactor);
				glm::vec3 interTranslate = glm::mix(srcTranslation, dstTranslation, blendFactor);
				glm::vec3 interScale = glm::mix(srcScale, dstScale, blendFactor);
				
				return glm::translate(glm::mat4(1.0f), interTranslate) *
					glm::toMat4(interOrientation) *
					glm::scale(glm::mat4(1.0f), interScale);
			}
			else
			{
				return glm::interpolate(src, dst, blendFactor);
			}
		}

		static glm::quat GetRotationFromMat4(const glm::mat4& srcMat)
		{
			glm::quat srcOrientation;
			glm::vec3 srcScale;
			glm::vec3 srcTranslation;
			glm::vec3 srcSkew;
			glm::vec4 srcPerspective;

			if (glm::decompose(srcMat, srcScale, srcOrientation, srcTranslation, srcSkew, srcPerspective))
			{
				return srcOrientation;
			}
			else
			{
				return glm::quat();
			}
		}
	};
}
