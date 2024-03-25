#pragma once

#include "pch.hpp"

namespace GLCore
{
	class Camera
	{
	public:
		enum class CameraMovDir { FORWARD = 1, BACKWARD, LEFT, RIGHT, UP, DOWN };

		Camera(glm::vec3 position = glm::vec3(0.0f));

		void processKeyboard(CameraMovDir direction, float deltaTime);
		void processMouse(double movX, double movY, double scrollY);

		inline glm::mat4 getViewMat() const { return glm::lookAt(m_cameraPos, m_cameraPos + m_cameraFront, m_cameraUp); }
		inline glm::mat4 getPerspectiveProjMat(int width, int height) const
		{
			return glm::perspective(glm::radians(m_fov),
									static_cast<float>(width) / static_cast<float>(height),
									0.01f, 100.0f);
		}
		inline glm::mat4 getOrthoProjMat(int width, int height) const
		{
			float wRadio = static_cast<float>(width) / static_cast<float>(width + height);
			float hRadio = static_cast<float>(height) / static_cast<float>(width + height);

			return glm::ortho(-1.0f, 1.0f * wRadio, -1.0f, 1.0f * hRadio, 0.01f, 100.0f);
		}
		inline void setCameraSpeed(float speed) { m_movementSpeed = speed; }
		inline float getCameraSpeed() const { return m_movementSpeed; }
		inline float getCameraFOV() const { return m_fov; }
		inline glm::vec3 getCameraPos() const { return m_cameraPos; }
		inline std::tuple<float, float> getEulerAngles() const { return {m_pitch, m_yaw}; }

	private:
		void updateCameraVectors();

	private:
		// 相机观察的方向
		glm::vec3 m_cameraPos;

		// 定义相机的三个轴
		glm::vec3 m_cameraFront;
		glm::vec3 m_cameraUp;
		glm::vec3 m_cameraRight;

		// 相机旋转
		float m_pitch = 0.0f;
		float m_yaw = 0.0f;
		glm::quat m_currentRotQuaternion;

		// 其他参数
		float m_movementSpeed;
		float m_mouseSensitivity;
		float m_fov;
	};
}