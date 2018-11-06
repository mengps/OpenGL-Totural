#ifndef MYWINDOW_H
#define MYWINDOW_H
#include "OpenGLWindow.h"
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/quaternion.hpp>

class MyRender;
class MyWindow : public OpenGLWindow
{
public:
	MyWindow();
	~MyWindow();

protected:
	void render();
	void resizeEvent(int width, int height);
	void mousePressedEvent(int button, int mods);
	void mouseReleasedEvent(int button, int mods);

private:
	MyRender *m_render;
	GLfloat m_angularSpeed = 0.0f;
	glm::vec2 m_mouseStartPos;
	glm::vec3 m_rotateAxis;
	glm::quat m_rotation;
};

#endif