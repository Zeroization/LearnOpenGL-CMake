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
		inline void setCameraSpeed(float speed) { m_movementSpeed = speed; }
		inline float getCameraSpeed() const { return m_movementSpeed; }
		inline float getCameraFOV() const { return m_fov; }
		inline glm::vec3 getCameraPos() const { return m_cameraPos; }
		inline std::tuple<float, float> getEulerAngles() const { return {m_pitch, m_yaw}; }

	private:
		void updateCameraVectors();

	private:
		// ����۲�ķ���
		glm::vec3 m_cameraPos;

		// ���������������
		glm::vec3 m_cameraFront;
		glm::vec3 m_cameraUp;
		glm::vec3 m_cameraRight;

		// �����ת
		float m_pitch = 0.0f;
		float m_yaw = 0.0f;
		glm::quat m_currentRotQuaternion;

		// ��������
		float m_movementSpeed;
		float m_mouseSensitivity;
		float m_fov;
	};
}