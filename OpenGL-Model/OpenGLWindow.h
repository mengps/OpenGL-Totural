#ifndef OPENGLWINDOW_H
#define OPENGLWINDOW_H
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>

using std::string;
class OpenGLWindow
{
public:
	enum OpenGLProfile
	{
		Core = 0x00032001,
		Compat
	};
	
	enum CursorMode
	{
		Normal = 0x00034001,
		Hidden,
		Disabled
	};

public:
	OpenGLWindow();
	virtual ~OpenGLWindow();

	void initializeWindow();

	int getFps() const { return m_fps; }
	glm::vec2 getCursorPos() const;
	void setWindowTitle(const string &title);
	void setWindowSize(int width, int height);
	void setCursorMode(CursorMode mode);

	void exec();
	void close();

	static void setOpenGLContextVersion(int major, int minor);
	static void setOpenGLProfile(OpenGLProfile profile);
	static void setSamples(int samples);

protected:
	virtual void render() { }

	virtual void closeEvent() { }
	virtual void focusEvent(int focused) { }
	virtual void minimizeEvent(int minimized) { }
	virtual void posEvent(int xpos, int ypos) { }
	virtual void updateEvent() { }
	virtual void resizeEvent(int width, int height) { glViewport(0, 0, width, height); }
	virtual void keyEvent(int key, int scancode, int action, int mods) { }
	virtual void mouseScrollEvent(double xoffset, double yoffset) { }
	virtual void mouseMoveEvent(double xpos, double ypos) { }
	virtual void mousePressedEvent(int button, int mods) { }
	virtual void mouseReleasedEvent(int button, int mods) { }
	virtual void charEvent(unsigned int codepoint) { }

private:
	static void closeCallback(GLFWwindow *window);
	static void focusCallback(GLFWwindow *window, int focused);
	static void minimizeCallback(GLFWwindow *window, int minimized);
	static void posCallback(GLFWwindow* window, int xpos, int ypos);
	static void updateCallback(GLFWwindow* window);
	static void resizeCallback(GLFWwindow* window, int width, int height);

	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
	static void mouseMoveCallBack(GLFWwindow *window, double xpos, double ypos);
	static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void charCallback(GLFWwindow* window, unsigned int codepoint);

	//static
	static int m_major, m_minor;
	static OpenGLProfile m_profile;
	static int m_samples;

private:
#ifdef _WIN32
	unsigned long long m_lastTime = 0;
#else
	double m_lastTime = 0.0;
#endif
	unsigned int m_fps = 0;
	string m_title = "OpenGL Window";
	GLFWwindow *m_window = nullptr;
};

#endif