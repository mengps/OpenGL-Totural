#ifndef MYRENDER_H
#define MYRENDER_H
#include "OpenGLRender.h"
#include "ShaderProgram.h"
#include <glm/matrix.hpp>

class Model;
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
	void initializeModel();
	void initializeLight();

private:
	GLuint m_lampVao, m_vbo, m_ebo;
	//0为聚光，1为定向光，2-5为点光源
	glm::vec3 m_lightColor[6];
	ShaderProgram m_modelProgram, m_lampProgram[6];
	glm::mat4x4 m_projection;
	Camera *m_camera;
	Model *m_model;
};

#endif
