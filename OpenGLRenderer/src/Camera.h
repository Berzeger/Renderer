#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

const float YAW = 270.0f;
const float PITCH = 0.0f;
const float FOV = 45.0f;
const float MOUSE_SENSITIVITY = 0.1f;
const float MOVEMENT_SPEED = 2.5f;

enum Camera_Movement
{
	FORWARD,
	BACKWARDS,
	LEFT,
	RIGHT
};

class Camera
{
private:
	glm::vec3 m_Position;
	glm::vec3 m_Front;
	glm::vec3 m_Up;
	glm::vec3 m_Right;
	glm::vec3 m_WorldUp;
	float m_Yaw, m_Pitch, m_Fov;
	float m_MovementSpeed;
	float m_MouseSensitivity;

	void updateCameraVectors();
public:

	void processKeyboard(Camera_Movement direction, float deltaTime);
	void processMouse(float xoffset, float yoffset);
	void processMouseScrollWheel(float yoffset);
	float getFov() const;
	glm::mat4 getViewMatrix() const;
	const glm::vec3& getPosition() const;
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
};
