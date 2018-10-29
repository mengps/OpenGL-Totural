#include "OpenGLWindow.h"
#include <iostream>
#include <sstream>

OpenGLWindow::OpenGLWindow()
	: m_title("OpenGL Window")
{
	initializeWindow();
}

OpenGLWindow::~OpenGLWindow()
{

}

void OpenGLWindow::closeCallback(GLFWwindow *window)
{
	OpenGLWindow *_this = static_cast<OpenGLWindow *>(glfwGetWindowUserPointer(window));
	_this->closeEvent();
}

void OpenGLWindow::focusCallback(GLFWwindow *window, int focused)
{
	OpenGLWindow *_this = static_cast<OpenGLWindow *>(glfwGetWindowUserPointer(window));
	_this->focusEvent(focused);
}

void OpenGLWindow::minimizeCallback(GLFWwindow *window, int minimized)
{
	OpenGLWindow *_this = static_cast<OpenGLWindow *>(glfwGetWindowUserPointer(window));
	_this->minimizeEvent(minimized);
}

void OpenGLWindow::posCallback(GLFWwindow* window, int xpos, int ypos)
{
	OpenGLWindow *_this = static_cast<OpenGLWindow *>(glfwGetWindowUserPointer(window));
	_this->posEvent(xpos, ypos);
}

void OpenGLWindow::updateCallback(GLFWwindow* window)
{
	OpenGLWindow *_this = static_cast<OpenGLWindow *>(glfwGetWindowUserPointer(window));
	_this->updateEvent();
}

void OpenGLWindow::resizeCallback(GLFWwindow *window, int width, int height)
{
	OpenGLWindow *_this = static_cast<OpenGLWindow *>(glfwGetWindowUserPointer(window));
	_this->resizeEvent(width, height);
}

void OpenGLWindow::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	OpenGLWindow *_this = static_cast<OpenGLWindow *>(glfwGetWindowUserPointer(window));
	_this->keyEvent(key, scancode, action, mods);
}

void OpenGLWindow::mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	OpenGLWindow *_this = static_cast<OpenGLWindow *>(glfwGetWindowUserPointer(window));
	_this->mouseScrollEvent(xoffset, yoffset);
}

void OpenGLWindow::charCallback(GLFWwindow* window, unsigned int codepoint)
{
	OpenGLWindow *_this = static_cast<OpenGLWindow *>(glfwGetWindowUserPointer(window));
	_this->charEvent(codepoint);
}

void OpenGLWindow::initializeWindow()
{
	glfwInit();
	m_window = glfwCreateWindow(600, 500, m_title.c_str(), nullptr, nullptr);
	glfwSetWindowUserPointer(m_window, this);

	glfwSetWindowCloseCallback(m_window, closeCallback);
	glfwSetWindowFocusCallback(m_window, focusCallback);
	glfwSetWindowIconifyCallback(m_window, minimizeCallback);
	glfwSetWindowPosCallback(m_window, posCallback);
	glfwSetWindowRefreshCallback(m_window, updateCallback);
	glfwSetWindowSizeCallback(m_window, resizeCallback);

	glfwSetKeyCallback(m_window, keyCallback);
	glfwSetScrollCallback(m_window, mouseScrollCallback);
	glfwSetCharCallback(m_window, charCallback);

	glfwMakeContextCurrent(m_window);
	if (!gladLoadGLLoader(GLADloadproc(glfwGetProcAddress)))
	{
		std::cerr << "Failed to Initliaze GLAD";
		close();
	}
}

void OpenGLWindow::setOpenGLContextVersion(int major, int minor)
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
}

void OpenGLWindow::setOpenGLProfile(OpenGLProfile profile)
{
	if (profile == OpenGLProfile::Core)
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	else if (profile == OpenGLProfile::Compat)
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
}

void OpenGLWindow::setWindowTitle(const string &title)
{
	m_title = title;
	std::ostringstream sout;
	sout << m_title << "---FPS : " << m_fps;
	glfwSetWindowTitle(m_window, sout.str().c_str());
}

void OpenGLWindow::setWindowSize(int width, int height)
{
	glfwSetWindowSize(m_window, width, height);
}

void OpenGLWindow::exec()
{
	//窗口启动时需要发送一个大小事件
	resizeEvent(600, 500);
#ifdef _WIN32
	m_lastTime = ::GetTickCount64();
#endif
	while (!glfwWindowShouldClose(m_window))
	{
		static unsigned int frame = 0;
		frame++;
		render();
		glfwSwapBuffers(m_window);

#ifdef _WIN32
		if (::GetTickCount64() - m_lastTime >= 1000)
		{
			m_lastTime = ::GetTickCount64();
			m_fps = frame;
			frame = 0;
			setWindowTitle(m_title);
		}
#endif
		glfwPollEvents();
	}
}

void OpenGLWindow::close()
{
	glfwTerminate();
}