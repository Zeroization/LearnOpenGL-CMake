#include "Camera.h"

Camera::Camera(glm::vec3 position, 
			   glm::vec3 up)
	: m_cameraFront(glm::vec3(0.0f, 0.0f, -1.0f)), m_movementSpeed(2.5f)
{
	m_cameraPos = position;
	m_cameraUp = up;
	updateCameraVectors();
}

void Camera::processKeyboard(CameraMovDir direction, float deltaTime)
{
    float velocity = deltaTime * m_movementSpeed;

    if (direction == CameraMovDir::FORWARD)
        m_cameraPos += m_cameraFront * velocity;
    if (direction == CameraMovDir::BACKWARD)
        m_cameraPos -= m_cameraFront * velocity;
    if (direction == CameraMovDir::LEFT)
        m_cameraPos -= m_cameraRight * velocity;
    if (direction == CameraMovDir::RIGHT)
        m_cameraPos += m_cameraRight * velocity;
}

void Camera::updateCameraVectors()
{
    // TODO: 四元数计算
    // glm::vec3 front;
    // front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    // front.y = sin(glm::radians(Pitch));
    // front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    // m_cameraFront = glm::normalize(front);

    // also re-calculate the Right and Up vector
    m_cameraRight = glm::normalize(glm::cross(m_cameraFront, glm::vec3(0.0f, 1.0f, 0.0f)));
    m_cameraUp = glm::normalize(glm::cross(m_cameraRight, m_cameraFront));
}
