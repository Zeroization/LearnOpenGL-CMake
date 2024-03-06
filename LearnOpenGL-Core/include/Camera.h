#pragma once

#include "pch.hpp"

class Camera
{
public:
	enum class CameraMovDir { FORWARD= 1, BACKWARD, LEFT, RIGHT };

	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f));

	void processKeyboard(CameraMovDir direction, float deltaTime);

	inline glm::mat4 getViewMat() const { return glm::lookAt(m_cameraPos, m_cameraPos + m_cameraFront, m_cameraUp); }
	inline void setCameraSpeed(float speed) { m_movementSpeed = speed; }
	inline float getCameraSpeed() const { return m_movementSpeed; }

private:
	void updateCameraVectors();

private:
	// 定义摄像机的位置、观察方向、上轴和右轴
	glm::vec3 m_cameraPos;
	glm::vec3 m_cameraFront;
	glm::vec3 m_cameraUp;
	glm::vec3 m_cameraRight;

	// 其他参数...
	float m_movementSpeed;
};
