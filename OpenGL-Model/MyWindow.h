#ifndef MYWINDOW_H
#define MYWINDOW_H
#include "OpenGLWindow.h"
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

class Camera;
class MyRender;
class MyWindow : public OpenGLWindow
{
public:
	MyWindow();
	~MyWindow();

protected:
	void render();
	void resizeEvent(int width, int height);
	void keyEvent(int key, int scancode, int action, int mods);
	void mouseMoveEvent(double xpos, double ypos);

private:
	Camera *m_camera;
	MyRender *m_render;
};

#endif