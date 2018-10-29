#include "MyWindow.h"
#include "MyRender.h"

MyWindow::MyWindow()
{
	m_render = new MyRender();
}

MyWindow::~MyWindow()
{
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