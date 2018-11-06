#include "MyWindow.h"
#include "MyRender.h"
#include <iostream>

MyWindow::MyWindow()
{
	m_render = new MyRender();
	//初始化为单位四元数
	m_rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
}

MyWindow::~MyWindow()
{
	if (m_render)
		delete m_render;
}

void MyWindow::render()
{
	m_angularSpeed *= 0.99f;

	if (m_angularSpeed < 0.01f)
		m_angularSpeed = 0.0f;
	else m_rotation = glm::angleAxis(m_angularSpeed, m_rotateAxis) * m_rotation;
	m_render->render(m_rotation);
}

void MyWindow::resizeEvent(int width, int height)
{
	OpenGLWindow::resizeEvent(width, height);
	m_render->resizeGL(width, height);
}

void MyWindow::mousePressedEvent(int button, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT)
		m_mouseStartPos = getCursorPos();
}

void MyWindow::mouseReleasedEvent(int button, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT)
	{
		if (getCursorPos() == m_mouseStartPos) return;
		glm::vec2 diff = getCursorPos() - m_mouseStartPos;
		glm::vec3 n = glm::normalize(glm::vec3(diff.y, diff.x, 0.0f));
		GLfloat acc = glm::distance(diff, glm::vec2(0.0f, 0.0f)) / 3000.0f;
		m_rotateAxis = glm::normalize(m_rotateAxis * m_angularSpeed + n * acc);
		m_angularSpeed += acc;

		std::cout << "旋转轴: (" << m_rotateAxis.x << "," <<  m_rotateAxis.y << "," << m_rotateAxis.z << ")" << std::endl;
		std::cout << std::endl;
	}
}