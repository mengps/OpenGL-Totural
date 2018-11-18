#ifndef MYRENDER_H
#define MYRENDER_H
#include "OpenGLRender.h"
#include <glm/matrix.hpp>

class Camera;
class MyRender : public OpenGLRender
{
public:
	MyRender(Camera *camera);
    ~MyRender();

	void render();
	void resizeGL(int w, int h);
	void initializeGL();
	void initializeShader();
	void initializeTexture();
	void initializeTriangle();

private:
	GLuint m_cubeVao, m_lampVao;
	GLuint m_vbo;
	GLuint m_ebo;
	GLuint m_cubeProgram, m_lampProgram;
	glm::mat4x4 m_projection;
	Camera *m_camera;
};

#endif
