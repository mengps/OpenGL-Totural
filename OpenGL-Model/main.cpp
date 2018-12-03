#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "glfw3.lib")
#pragma comment (lib, "assimp.lib")

#include "MyWindow.h"

int main(int argc, char **argv)
{
	OpenGLWindow::setSamples(16);
	MyWindow window;
	window.exec();
}