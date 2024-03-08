#pragma once

#include "pch.hpp"

class Camera
{
public:
	enum class CameraMovDir { FORWARD= 1, BACKWARD, LEFT, RIGHT };

	Camera();

	void processKeyboard(CameraMovDir direction, float deltaTime);
	void processMouse(double movX, double movY, double scrollY);

	inline glm::mat4 getViewMat() const { return glm::lookAt(m_cameraPos, m_cameraPos + m_cameraFront, m_cameraUp); }
	inline void setCameraSpeed(float speed) { m_movementSpeed = speed; }
	inline float getCameraSpeed() const { return m_movementSpeed; }
	inline float getCameraFOV() const { return m_fov; }
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
