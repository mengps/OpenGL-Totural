#include "MyWindow.h"
#include "MyRender.h"
#include "Camera.h"
#include <iostream>

MyWindow::MyWindow()
{
	m_camera = new Camera();
	m_render = new MyRender(m_camera);
	//关闭光标显示
	setCursorMode(Disabled);
}

MyWindow::~MyWindow()
{
	if (m_camera)
		delete m_camera;
	if (m_render)
		delete m_render;
}

void MyWindow::render()
{
	m_render->render();
}

void MyWindow::resizeEvent(int width, int height)
{
	OpenGLWindow::resizeEvent(width, height);
	m_render->resizeGL(width, height);
}

void MyWindow::keyEvent(int key, int scancode, int action, int mods)
{
	switch (key)
	{
	case GLFW_KEY_ESCAPE:
		close();
		break;
	case GLFW_KEY_W:
		m_camera->move(Camera::Front, 0.15f);
		break;
	case GLFW_KEY_S:
		m_camera->move(Camera::Back, 0.15f);
		break;
	case GLFW_KEY_A:
		m_camera->move(Camera::Left, 0.15f);
		break;
	case GLFW_KEY_D:
		m_camera->move(Camera::Right, 0.15f);
		break;
	}
}

void MyWindow::mouseMoveEvent(double xpos, double ypos)
{
	static bool first = true;
	static double lastX, lastY;
	if (first)
	{
		lastX = xpos;
		lastY = ypos;
		first = false;
	}

	double xoffset = xpos - lastX;
	double yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	m_camera->rotate(Camera::Vertical, (float)yoffset * 0.02f);
	m_camera->rotate(Camera::Horizontal, (float)xoffset * 0.02f);
}