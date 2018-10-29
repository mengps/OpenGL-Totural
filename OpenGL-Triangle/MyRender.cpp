#include "MyRender.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct VertexData
{
	glm::vec3 postion;
	glm::vec3 color;
};

MyRender::MyRender()
{
	initializeGL();
}

MyRender::~MyRender()
{
	glDeleteBuffers(1, &m_vbo);
	glDeleteProgram(m_program);
}

void MyRender::render()
{
	static GLfloat angle = 0.0f;
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	angle += 1.0f;
	glm::mat4 modelMatrix(1.0f);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, -5.0f));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));

	glUseProgram(m_program);
	GLuint mvp = glGetUniformLocation(m_program, "mvp");
	glUniformMatrix4fv(mvp, 1, GL_FALSE, glm::value_ptr(m_projection * modelMatrix));

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void MyRender::resizeGL(int w, int h)
{
	GLfloat aspect = (GLfloat)w / (GLfloat)h;
	m_projection = glm::perspective(glm::radians(30.0f), aspect, 1.0f, 10.0f);
}

void MyRender::initializeGL()
{
	initializeShader();
	initializeTriangle();
}

void MyRender::initializeShader()
{
	GLuint vertexShader = compileShaderFile(Vertex, "../GLSL/vertex_glsl.vert");
	GLuint fragmentShader = compileShaderFile(Fragment, "../GLSL/fragment_glsl.frag");

	m_program = glCreateProgram();
	glAttachShader(m_program, vertexShader);
	glAttachShader(m_program, fragmentShader);
	glLinkProgram(m_program);

	int success;
	char infoLog[512];
	glGetProgramiv(m_program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(m_program, sizeof(infoLog), nullptr, infoLog);
		std::cerr << "Shader Program Linking Error :" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void MyRender::initializeTriangle()
{
	VertexData vertices[] =
	{
		{ glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0.9f, 0.0f, 0.9f) },
		{ glm::vec3( 0.0f,  0.5f, 0.0f), glm::vec3(0.9f, 0.9f, 0.0f) },
		{ glm::vec3( 0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.9f, 0.9f) }
	};

	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	int location = 0;
	glVertexAttribPointer(location, 3, GL_FLOAT, GL_TRUE, sizeof(VertexData), (void *)0);
	glEnableVertexAttribArray(location);
	glVertexAttribPointer(location + 1, 3, GL_FLOAT, GL_TRUE, sizeof(VertexData), (void *)(sizeof(glm::vec3)));
	glEnableVertexAttribArray(location + 1);
}