#ifndef MYRENDER_H
#define MYRENDER_H
#include "OpenGLRender.h"
#include <glm/matrix.hpp>
#include <glm/gtc/quaternion.hpp>

class MyRender : public OpenGLRender
{
public:
	MyRender();
    ~MyRender();

	void render(const glm::quat &rotation);
	void resizeGL(int w, int h);
	void initializeGL();
	void initializeShader();
	void initializeTexture();
	void initializeTriangle();

private:
	GLuint m_vao;
	GLuint m_vbo;
	GLuint m_ebo;
	GLuint m_texture1, m_texture2;
	GLuint m_program;
	glm::mat4x4 m_projection;
};

#endif
