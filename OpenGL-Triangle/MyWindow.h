#ifndef MYWINDOW_H
#define MYWINDOW_H
#include "OpenGLWindow.h"

class MyRender;
class MyWindow : public OpenGLWindow
{
public:
	MyWindow();
	~MyWindow();

protected:
	void render();
	void resizeEvent(int width, int height);

private:
	MyRender *m_render;
};

#endif