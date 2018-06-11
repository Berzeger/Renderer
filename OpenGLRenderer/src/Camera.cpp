#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) :
	m_Front(glm::vec3(0.0f, 0.0f, -1.0f)),
	m_MovementSpeed(MOVEMENT_SPEED),
	m_MouseSensitivity(MOUSE_SENSITIVITY),
	m_Fov(FOV),
	m_Yaw(yaw),
	m_Pitch(pitch),
	m_WorldUp(up),
	m_Position(position)
{
	updateCameraVectors();
}

void Camera::updateCameraVectors()
{
	glm::vec3 front;
	front.x = cos(glm::radians(m_Pitch)) * cos(glm::radians(m_Yaw));
	front.y = sin(glm::radians(m_Pitch));
	front.z = cos(glm::radians(m_Pitch)) * sin(glm::radians(m_Yaw));
	m_Front = glm::normalize(front);
	m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
	m_Up = glm::normalize(glm::cross(m_Right, m_Front));
}

void Camera::processKeyboard(Camera_Movement direction, float deltaTime)
{
	float velocity = m_MovementSpeed * deltaTime;
	if (direction == FORWARD)
	{
		m_Position += velocity * m_Front;
	}
	if (direction == BACKWARDS)
	{
		m_Position -= velocity * m_Front;
	}
	if (direction == LEFT)
	{
		m_Position -= velocity * m_Right;
	}
	if (direction == RIGHT)
	{
		m_Position += velocity * m_Right;
	}
}

void Camera::processMouse(float xoffset, float yoffset)
{
	xoffset *= m_MouseSensitivity;
	yoffset *= m_MouseSensitivity;

	m_Yaw += xoffset;
	m_Pitch += yoffset;

	if (m_Pitch > 89.0f)
	{
		m_Pitch = 89.0f;
	}

	if (m_Pitch < -89.0f)
	{
		m_Pitch = -89.0f;
	}

	updateCameraVectors();
}

void Camera::processMouseScrollWheel(float yoffset)
{
	if (m_Fov >= 1.0f && m_Fov <= 45.0f)
	{
		m_Fov -= yoffset;
	}

	if (m_Fov <= 1.0f)
	{
		m_Fov = 1.0f;
	}

	if (m_Fov >= 45.0f)
	{
		m_Fov = 45.0f;
	}
}

float Camera::getFov() const
{
	return m_Fov;
}

const glm::vec3& Camera::getPosition() const
{
	return m_Position;
}

glm::mat4 Camera::getViewMatrix() const
{
	return glm::lookAt(m_Position, m_Position + m_Front, m_Up);
}
