#include "Camera.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(const glm::vec3 &cameraPos, const glm::vec3 &cameraFront, const glm::vec3 &cameraUp)
	: m_cameraPos(cameraPos),
	  m_cameraFront(cameraFront),
	  m_cameraUp(cameraUp)
{

}

Camera::~Camera()
{

}

glm::mat4 Camera::getViewMatrix() const
{
	return glm::lookAt(m_cameraPos, m_cameraPos + m_cameraFront, m_cameraUp);
}

void Camera::move(MoveDirection direction, float step)
{
	switch (direction)
	{
	case Front:
		m_cameraPos += step * m_cameraFront;
		break;

	case Back:
		m_cameraPos -= step * m_cameraFront;
		break;

	case Left:
		m_cameraPos -= step * glm::normalize(glm::cross(m_cameraFront, m_cameraUp));
		break;

	case Right:
		m_cameraPos += step * glm::normalize(glm::cross(m_cameraFront, m_cameraUp));
		break;
	default:
		break;
	}
}

void Camera::rotate(RotateDirection direction, float angle)
{
	static float pitch = 0.0f;
	static float yaw = -90.0f;

	if (direction == Vertical)
	{
		pitch += angle;
		/*
		if (pitch > 89.9f)
			pitch = 89.9f;
		if (pitch < -89.9f)
			pitch = -89.9f;*/
	}
	else if (direction == Horizontal)
		yaw += angle;

	float x = cos(glm::radians(yaw));
	float y = sin(glm::radians(pitch));
	float z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	m_cameraFront = glm::normalize(glm::vec3(x, y, z));
}