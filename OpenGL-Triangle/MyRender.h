#ifndef MYRENDER_H
#define MYRENDER_H
#include "OpenGLRender.h"
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

class MyRender : public OpenGLRender
{
public:
	MyRender();
    ~MyRender();

	void render();
	void resizeGL(int w, int h);
	void initializeGL();
	void initializeShader();
	void initializeTriangle();

private:
	GLuint m_vbo;
	GLuint m_program;
	glm::mat4x4 m_projection;
};

#endif
