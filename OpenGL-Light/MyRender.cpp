#include "MyRender.h"
#include "Camera.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct VertexData
{
	glm::vec3 postion;
	glm::vec3 color;
	glm::vec3 normal;
};

MyRender::MyRender(Camera *camera)
{
	m_camera = camera;
	initializeGL();
}

MyRender::~MyRender()
{
	glDeleteVertexArrays(1, &m_cubeVao);
	glDeleteVertexArrays(1, &m_lampVao);
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_ebo);
	glDeleteProgram(m_cubeProgram);
	glDeleteProgram(m_lampProgram);
}

void MyRender::render()
{
	//虽然默认为黑色, 这里还是显式清除为黑色
	static glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	static glm::vec4 lightPosition = glm::vec4(0.0f, 0.0f, 2.0f, 1.0f);
	static float angle = 0.0f;

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 modelMatrix(1.0f);
	modelMatrix = glm::rotate(modelMatrix, angle += 0.01F, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 lightMatrix(1.0f);
	lightMatrix = glm::rotate(lightMatrix, 0.04f, glm::vec3(1.0f, 1.0f, 1.0f));
	lightPosition = lightMatrix * lightPosition;
	
	glUseProgram(m_cubeProgram);
	glm::mat3 normalMatrix = glm::transpose(glm::inverse(modelMatrix));
	GLuint normalLocation = glGetUniformLocation(m_cubeProgram, "normalMatrix");
	glUniformMatrix3fv(normalLocation, 1, GL_FALSE, glm::value_ptr(normalMatrix));	

	GLuint modelLocation = glGetUniformLocation(m_cubeProgram, "model");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	GLuint viewLocation = glGetUniformLocation(m_cubeProgram, "view");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(m_camera->getViewMatrix()));
	GLuint projectionLocation = glGetUniformLocation(m_cubeProgram, "projection");
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(m_projection));

	GLuint viewPositionLocation = glGetUniformLocation(m_cubeProgram, "viewPosition");
	glUniform3fv(viewPositionLocation, 1, glm::value_ptr(m_camera->getCameraPos()));
	GLuint lightColorLocation = glGetUniformLocation(m_cubeProgram, "lightColor");
	glUniform3fv(lightColorLocation, 1, glm::value_ptr(lightColor));
	GLuint lightPositionLocation = glGetUniformLocation(m_cubeProgram, "lightPosition");
	glUniform3fv(lightPositionLocation, 1, glm::value_ptr(lightPosition));

	glBindVertexArray(m_cubeVao);
	glDrawElements(GL_TRIANGLE_STRIP, 34, GL_UNSIGNED_SHORT, nullptr);

	glUseProgram(m_lampProgram);
	modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(lightPosition));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(0.08f));
	modelLocation = glGetUniformLocation(m_lampProgram, "model");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	viewLocation = glGetUniformLocation(m_lampProgram, "view");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(m_camera->getViewMatrix()));
	projectionLocation = glGetUniformLocation(m_lampProgram, "projection");
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(m_projection));

	lightColorLocation = glGetUniformLocation(m_lampProgram, "lightColor");
	glUniform3fv(lightColorLocation, 1, glm::value_ptr(lightColor));

	glBindVertexArray(m_lampVao);
	glDrawElements(GL_TRIANGLE_STRIP, 34, GL_UNSIGNED_SHORT, nullptr);
}

void MyRender::resizeGL(int w, int h)
{
	GLfloat aspect = (GLfloat)w / (GLfloat)h;
	m_projection = glm::perspective(glm::radians(45.0f), aspect, 1.0f, 40.0f);
}

void MyRender::initializeGL()
{
	initializeShader();
	initializeTexture();
	initializeTriangle();

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
}

void MyRender::initializeShader()
{
	GLuint vertexShader = compileShaderFile(Vertex, "../GLSL/vertex_glsl.vert");
	GLuint fragmentShader = compileShaderFile(Fragment, "../GLSL/fragment_glsl.frag");

	m_cubeProgram = glCreateProgram();
	glAttachShader(m_cubeProgram, vertexShader);
	glAttachShader(m_cubeProgram, fragmentShader);
	glLinkProgram(m_cubeProgram);

	int success;
	char infoLog[512];
	glGetProgramiv(m_cubeProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(m_cubeProgram, sizeof(infoLog), nullptr, infoLog);
		std::cerr << "Cube Shader Program Linking Error :" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	GLuint lamp_vertexShader = compileShaderFile(Vertex, "../GLSL/lamp_vertex_glsl.vert");
	GLuint lamp_fragmentShader = compileShaderFile(Fragment, "../GLSL/lamp_fragment_glsl.frag");

	m_lampProgram = glCreateProgram();
	glAttachShader(m_lampProgram, lamp_vertexShader);
	glAttachShader(m_lampProgram, lamp_fragmentShader);
	glLinkProgram(m_lampProgram);

	glGetProgramiv(m_lampProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(m_lampProgram, sizeof(infoLog), nullptr, infoLog);
		std::cerr << "Lamp Shader Program Linking Error :" << infoLog << std::endl;
	}

	glDeleteShader(lamp_vertexShader);
	glDeleteShader(lamp_fragmentShader);
}

void MyRender::initializeTexture()
{

}

void MyRender::initializeTriangle()
{
	glm::vec3 normal1 = glm::cross(glm::vec3(0.75f, -0.75f, 0.75f) - glm::vec3(-0.75f, -0.75f, 0.75f),
		glm::vec3(-0.375f, 0.75f, 0.375f) - glm::vec3(-0.75f, -0.75f, 0.75f));
	glm::vec3 normal2 = glm::cross(glm::vec3(0.75f, -0.75f, -0.75f) - glm::vec3(0.75f, -0.75f, 0.75f),
		glm::vec3(0.375f, 0.75f, 0.375f) - glm::vec3(0.75f, -0.75f, 0.75f));
	glm::vec3 normal3 = glm::cross(glm::vec3(-0.75f, -0.75f, -0.75f) - glm::vec3(0.75f, -0.75f, -0.75f),
		glm::vec3(0.375f, 0.75f, -0.375f) - glm::vec3(0.75f, -0.75f, -0.75f));
	glm::vec3 normal4 = glm::cross(glm::vec3(-0.75f, -0.75f, 0.75f) - glm::vec3(-0.75f, -0.75f, -0.75f),
		glm::vec3(-0.375f, 0.75f, -0.375f) - glm::vec3(-0.75f, -0.75f, -0.75f));
	glm::vec3 normal5 = glm::cross(glm::vec3(0.375f, 0.75f, 0.375f) - glm::vec3(-0.375f, 0.75f, 0.375f),
		glm::vec3(-0.375f, 0.75f, -0.375f) - glm::vec3(-0.375f, 0.75f, 0.375f));
	glm::vec3 normal6 = glm::cross(glm::vec3(-0.75f, -0.75f, 0.75f) - glm::vec3(0.75f, -0.75f, 0.75f),
		glm::vec3(0.75f, -0.75f, -0.75f) - glm::vec3(0.75f, -0.75f, 0.75f));

	VertexData vertices[] =
	{
		{ glm::vec3( -0.75f, -0.75f,  0.75f), glm::vec3(0.8f, 0.8f, 0.0f), normal1 },
		{ glm::vec3(  0.75f, -0.75f,  0.75f), glm::vec3(0.8f, 0.8f, 0.0f), normal1 },
		{ glm::vec3(-0.375f,  0.75f, 0.375f), glm::vec3(0.0f, 0.8f, 0.8f), normal1 },
		{ glm::vec3( 0.375f,  0.75f, 0.375f), glm::vec3(0.0f, 0.8f, 0.8f), normal1 },

		{ glm::vec3( 0.75f, -0.75f,   0.75f), glm::vec3(0.8f, 0.8f, 0.0f), normal2 },
		{ glm::vec3( 0.75f, -0.75f,  -0.75f), glm::vec3(0.8f, 0.8f, 0.0f), normal2 },
		{ glm::vec3(0.375f,  0.75f,  0.375f), glm::vec3(0.0f, 0.8f, 0.8f), normal2 },
		{ glm::vec3(0.375f,  0.75f, -0.375f), glm::vec3(0.0f, 0.8f, 0.8f), normal2 },

		{ glm::vec3(  0.75f, -0.75f,  -0.75f), glm::vec3(0.8f, 0.8f, 0.0f), normal3 },
		{ glm::vec3( -0.75f, -0.75f,  -0.75f), glm::vec3(0.8f, 0.8f, 0.0f), normal3 },
		{ glm::vec3( 0.375f,  0.75f, -0.375f), glm::vec3(0.0f, 0.8f, 0.8f), normal3 },
		{ glm::vec3(-0.375f,  0.75f, -0.375f), glm::vec3(0.0f, 0.8f, 0.8f), normal3 },

		{ glm::vec3( -0.75f, -0.75f,  -0.75f), glm::vec3(0.8f, 0.8f, 0.0f), normal4 },
		{ glm::vec3( -0.75f, -0.75f,   0.75f), glm::vec3(0.8f, 0.8f, 0.0f), normal4 },
		{ glm::vec3(-0.375f,  0.75f, -0.375f), glm::vec3(0.0f, 0.8f, 0.8f), normal4 },
		{ glm::vec3(-0.375f,  0.75f,  0.375f), glm::vec3(0.0f, 0.8f, 0.8f), normal4 },

		{ glm::vec3(-0.375f, 0.75f,  0.375f), glm::vec3(0.8f, 0.8f, 0.0f), normal5 },
		{ glm::vec3( 0.375f, 0.75f,  0.375f), glm::vec3(0.8f, 0.8f, 0.0f), normal5 },
		{ glm::vec3(-0.375f, 0.75f, -0.375f), glm::vec3(0.0f, 0.8f, 0.8f), normal5 },
		{ glm::vec3( 0.375f, 0.75f, -0.375f), glm::vec3(0.0f, 0.8f, 0.8f), normal5 },

		{ glm::vec3( 0.75f, -0.75f,  0.75f), glm::vec3(0.8f, 0.8f, 0.0f), normal6 },
		{ glm::vec3(-0.75f, -0.75f,  0.75f), glm::vec3(0.8f, 0.8f, 0.0f), normal6 },
		{ glm::vec3( 0.75f, -0.75f, -0.75f), glm::vec3(0.0f, 0.8f, 0.8f), normal6 },
		{ glm::vec3(-0.75f, -0.75f, -0.75f), glm::vec3(0.0f, 0.8f, 0.8f), normal6 }
	};

	GLushort indices[] =
	{
		 0,  1,  2,  3,  3,
		 4,  4,  5,  6,  7, 7,
		 8,  8,  9, 10, 11, 11,
		12, 12, 13, 14, 15, 15,
		16, 16, 17, 18, 19, 19,
		20, 20, 21, 22, 23
	};

	glGenBuffers(1, &m_cubeVao);
	glBindVertexArray(m_cubeVao);

	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &m_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	int location = 0;
	glVertexAttribPointer(location, 3, GL_FLOAT, GL_TRUE, sizeof(VertexData), (void *)0);
	glEnableVertexAttribArray(location);
	glVertexAttribPointer(location + 1, 3, GL_FLOAT, GL_TRUE, sizeof(VertexData), (void *)(sizeof(glm::vec3)));
	glEnableVertexAttribArray(location + 1);
	glVertexAttribPointer(location + 2, 3, GL_FLOAT, GL_TRUE, sizeof(VertexData), (void *)(sizeof(glm::vec3) * 2));
	glEnableVertexAttribArray(location + 2);

	glGenBuffers(1, &m_lampVao);
	glBindVertexArray(m_lampVao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	glVertexAttribPointer(location, 3, GL_FLOAT, GL_TRUE, sizeof(VertexData), (void *)0);
	glEnableVertexAttribArray(location);
}