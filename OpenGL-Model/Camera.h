#ifndef CAMERA_H
#define CAMERA_H
#include <glm/matrix.hpp>

class Camera
{
public:
	enum MoveDirection
	{
		Front = 0,
		Back,
		Left,
		Right
	};

	enum RotateDirection
	{
		Vertical = 0,
		Horizontal
	};

public:
	Camera();
	Camera(const glm::vec3 &cameraPos, const glm::vec3 &cameraFront, const glm::vec3 &cameraUp);
	~Camera();

	void setCameraPos(const glm::vec3 &cameraPos) { m_cameraPos = cameraPos; }
	glm::vec3 getCameraPos() const { return m_cameraPos; }
	void setCameraFront(const glm::vec3 &cameraFront) { m_cameraFront = cameraFront; }
	glm::vec3 getCameraFront() const { return m_cameraFront; }
	glm::mat4 getViewMatrix() const;

	//在Front, Back, Left, Right四个方向上移动摄像机, 距离为step
	void move(MoveDirection direction, float step);
	//在Vertical, Horizontal两个方向上旋转摄像机, 角度为angle
	void rotate(RotateDirection direction, float angle);

private:
	glm::vec3 m_cameraPos = glm::vec3(0.0f, 3.0f, 5.0f);
	glm::vec3 m_cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 m_cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
};

#endif