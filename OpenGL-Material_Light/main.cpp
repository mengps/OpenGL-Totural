#include "MyWindow.h"

int main(int argc, char **argv)
{
	OpenGLWindow::setSamples(16);
	MyWindow window;
	window.setWindowTitle("OpenGL-Material_Light");
	window.exec();
}