#ifndef OPENGLRENDER_H
#define OPENGLRENDER_H
#include <glad/glad.h>

class OpenGLRender
{
public:	
	virtual void render() = 0;
	virtual void resizeGL(int w, int h) = 0;
	virtual void initializeGL() = 0;
	virtual void initializeShader() = 0;
};

#endif