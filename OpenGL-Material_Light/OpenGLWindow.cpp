#include "OpenGLWindow.h"
#include <iostream>
#include <sstream>

int OpenGLWindow::m_major = 3;
int OpenGLWindow::m_minor = 3;
OpenGLWindow::OpenGLProfile OpenGLWindow::m_profile = Compat;
int OpenGLWindow::m_samples = 4;

OpenGLWindow::OpenGLWindow()
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

void OpenGLWindow::mouseMoveCallBack(GLFWwindow *window, double xpos, double ypos)
{
	OpenGLWindow *_this = static_cast<OpenGLWindow *>(glfwGetWindowUserPointer(window));
	_this->mouseMoveEvent(xpos, ypos);
}

void OpenGLWindow::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	OpenGLWindow *_this = static_cast<OpenGLWindow *>(glfwGetWindowUserPointer(window));
	if (action == GLFW_PRESS)
		_this->mousePressedEvent(button, mods);
	else if (action == GLFW_RELEASE)
		_this->mouseReleasedEvent(button, mods);
}

void OpenGLWindow::charCallback(GLFWwindow* window, unsigned int codepoint)
{
	OpenGLWindow *_this = static_cast<OpenGLWindow *>(glfwGetWindowUserPointer(window));
	_this->charEvent(codepoint);
}

void OpenGLWindow::initializeWindow()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, m_major);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, m_minor);
	glfwWindowHint(GLFW_OPENGL_PROFILE, m_profile);
	glfwWindowHint(GLFW_SAMPLES, m_samples);

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
	glfwSetCursorPosCallback(m_window, mouseMoveCallBack);
	glfwSetMouseButtonCallback(m_window, mouseButtonCallback);
	glfwSetCharCallback(m_window, charCallback);

	glfwMakeContextCurrent(m_window);
	if (!gladLoadGLLoader(GLADloadproc(glfwGetProcAddress)))
	{
		std::cerr << "Failed to Initliaze GLAD" << std::endl;
		close();
	}
}

glm::vec2 OpenGLWindow::getCursorPos() const
{
	double xpos, ypos;
	glfwGetCursorPos(m_window, &xpos, &ypos);

	return glm::vec2(xpos, ypos);
}


void OpenGLWindow::setOpenGLContextVersion(int major, int minor)
{
	m_major = major;
	m_minor = minor;
}

void OpenGLWindow::setOpenGLProfile(OpenGLProfile profile)
{
	m_profile = profile;
}

void OpenGLWindow::setSamples(int samples)
{
	m_samples = samples;
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

void OpenGLWindow::setCursorMode(CursorMode mode)
{
	glfwSetInputMode(m_window, GLFW_CURSOR, mode);
}

void OpenGLWindow::exec()
{
	//窗口启动时需要发送一个大小事件
	resizeEvent(600, 500);
#ifdef _WIN32
	m_lastTime = ::GetTickCount64();
#else 
	m_lastTime = glfwGetTime();
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
#else
		if (glfwGetTime() - m_lastTime > 1.0000)
		{
			m_lastTime = glfwGetTime();
#endif
			m_fps = frame;
			frame = 0;
			setWindowTitle(m_title);
		}
		glfwPollEvents();
	}
}

void OpenGLWindow::close()
{
	glfwSetWindowShouldClose(m_window, true);
}