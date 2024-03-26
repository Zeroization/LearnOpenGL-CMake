#include "Core/Camera.h"

namespace GLCore
{
    Camera::Camera(glm::vec3 position)
        : m_cameraPos(position),
        m_cameraFront(0.0f, 0.0f, -1.0f),
        m_cameraUp(0.0f, 1.0f, 0.0f),
        m_cameraRight(1.0f, 0.0f, 0.0f),
        m_movementSpeed(2.5f), m_mouseSensitivity(0.05f), m_fov(45.0f)
    {
        glm::quat rot_up = glm::angleAxis(glm::radians(0.0f), m_cameraUp);
        glm::quat rot_right = glm::angleAxis(glm::radians(0.0f), m_cameraRight);
        m_currentRotQuaternion = rot_right * rot_up;

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
        if (direction == CameraMovDir::UP)
            m_cameraPos += m_cameraUp * velocity;
        if (direction == CameraMovDir::DOWN)
            m_cameraPos -= m_cameraUp * velocity;
    }

    void Camera::processMouse(double movX, double movY, double scrollY)
    {
        // 处理旋转
        movX *= m_mouseSensitivity;
        movY *= m_mouseSensitivity;

        m_yaw = static_cast<float>(movX);
        m_pitch = static_cast<float>(movY);
        updateCameraVectors();

        // 处理缩放
        m_fov -= static_cast<float>(scrollY);
        if (m_fov < 1.0f)
            m_fov = 1.0f;
        if (m_fov > 45.0f)
            m_fov = 45.0f;
    }

    void Camera::updateCameraVectors()
    {
        glm::quat rot_up = glm::angleAxis(glm::radians(-m_yaw), m_cameraUp);
        glm::quat rot_right = glm::angleAxis(glm::radians(m_pitch), m_cameraRight);
        m_currentRotQuaternion = rot_right * rot_up;

        glm::quat up(0.0f, m_cameraUp);
        glm::quat right(0.0f, m_cameraRight);
        up = m_currentRotQuaternion * up * glm::conjugate(m_currentRotQuaternion);
        right = m_currentRotQuaternion * right * glm::conjugate(m_currentRotQuaternion);

        m_cameraUp = glm::normalize(glm::vec3(up.x, up.y, up.z));
        m_cameraRight = glm::normalize(glm::vec3(right.x, right.y, right.z));
        m_cameraFront = glm::normalize(glm::cross(m_cameraUp, m_cameraRight));
    }
}