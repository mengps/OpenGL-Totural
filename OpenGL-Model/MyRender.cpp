#include "MyRender.h"
#include "Camera.h"
#include "Image.h"
#include "Model.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/random.hpp>

//生成10个随机的位置
static glm::vec3 modelPosition[10];

MyRender::MyRender(Camera *camera)
{
	srand((unsigned int)time(0));

	m_camera = camera;
	m_lightColor[0] = glm::vec3(1.0f, 1.0f, 1.0f);
	m_lightColor[1] = glm::vec3(1.0f, 1.0f, 1.0f);
	m_lightColor[2] = glm::vec3(1.0f, 1.0f, 0.0f);
	m_lightColor[3] = glm::vec3(0.0f, 1.0f, 1.0f);
	m_lightColor[4] = glm::vec3(1.0f, 0.0f, 0.0f);
	m_lightColor[5] = glm::vec3(1.0f, 0.0f, 1.0f);
	for (int i = 0; i < 10; i++)
		modelPosition[i] = 5.0f * glm::ballRand(1.0f);
	initializeGL();
}

MyRender::~MyRender()
{
	if (m_model) delete m_model;
	glDeleteVertexArrays(1, &m_lampVao);
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_ebo);
}

void MyRender::render()
{
	static float angle = 0.0f;
	angle += 0.01f;
	glm::vec3 lightRotation = glm::vec3((sin(angle) + 1.0f) / 2.0f + 0.1f,
		(cos(1.7f * angle) + 1.0f) / 2.0f + 0.1f,
		(sin(2.9f * angle) + 1.0f) / 2.0f + 0.1f);
	static glm::vec4 lightPosition[6] =
	{
		//0，1用来占位的
		glm::vec4(0.0f),
		glm::vec4(0.0f),
		glm::vec4(5.7f, 0.2f, 1.0f, 1.0f),
		glm::vec4(6.3f, -2.3f, -3.0f, 1.0f),
		glm::vec4(-2.0f, 5.0f, -3.0f, 1.0f),
		glm::vec4(0.0f, 0.0f, -7.0f, 1.0f)
	};

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//绘制模型
	{
		glm::mat4 modelMatrix(1.0f);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, 0.0f)); 
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f));	

		glm::mat3 normalMatrix = glm::transpose(glm::inverse(modelMatrix));
		//设置顶点着色器的uniform
		m_modelProgram.use();
		m_modelProgram.setValue("normalMatrix", normalMatrix);
		m_modelProgram.setValue("model", modelMatrix);
		m_modelProgram.setValue("view", m_camera->getViewMatrix());
		m_modelProgram.setValue("projection", m_projection);

		//设置片元着色器的uniform
		m_modelProgram.setValue("viewPos", m_camera->getCameraPos());
		m_modelProgram.setValue("lights[0].position", m_camera->getCameraPos());
		m_modelProgram.setValue("lights[0].direction", m_camera->getCameraFront());
		m_modelProgram.setValue("lights[0].cutOff", glm::cos(glm::radians(12.0f)));
		m_modelProgram.setValue("lights[0].outerCutOff", glm::cos(glm::radians(14.5f)));
		m_modelProgram.setValue("lights[0].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		m_modelProgram.setValue("lights[0].diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
		m_modelProgram.setValue("lights[0].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		m_modelProgram.setValue("lights[0].constant", 1.0f);
		m_modelProgram.setValue("lights[0].linear", 0.09f);
		m_modelProgram.setValue("lights[0].quadratic", 0.032f);

		m_modelProgram.setValue("lights[1].direction", glm::vec3(0.0f, 10.0f, 0.0f));
		m_modelProgram.setValue("lights[1].ambient", glm::vec3(0.1f, 0.1f, 0.1f));
		m_modelProgram.setValue("lights[1].diffuse", glm::vec3(0.4f, 0.4f, 0.4f));
		m_modelProgram.setValue("lights[1].specular", glm::vec3(0.8f, 0.8f, 0.8f));

		for (int i = 2; i < 6; i++)
		{
			glm::mat4 lightMatrix(1.0f);
			lightMatrix = glm::rotate(lightMatrix, 0.008f, lightRotation);
			lightPosition[i] = lightMatrix * lightPosition[i];
			string num = std::to_string(i);
			m_modelProgram.setValue("lights[" + num + "].position", glm::vec3(lightPosition[i]));
			m_modelProgram.setValue("lights[" + num + "].ambient", 0.05f * m_lightColor[i]);
			m_modelProgram.setValue("lights[" + num + "].diffuse", 0.5f * m_lightColor[i]);
			m_modelProgram.setValue("lights[" + num + "].specular", 1.0f * m_lightColor[i]);
			m_modelProgram.setValue("lights[" + num + "].constant", 1.0f);
			m_modelProgram.setValue("lights[" + num + "].linear", 0.09f);
			m_modelProgram.setValue("lights[" + num + "].quadratic", 0.032f);
		}

		m_model->render(&m_modelProgram);
	}

	//绘制灯
	for (int i = 2; i < 6; i++)
	{
		m_lampProgram[i].use();
		glm::mat4 modelMatrix(1.0f);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(lightPosition[i]));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.06f));

		m_lampProgram[i].setValue("model", modelMatrix);
		m_lampProgram[i].setValue("view", m_camera->getViewMatrix());
		m_lampProgram[i].setValue("projection", m_projection);

		glBindVertexArray(m_lampVao);
		glDrawElements(GL_TRIANGLE_STRIP, 34, GL_UNSIGNED_SHORT, nullptr);
	}
}

void MyRender::resizeGL(int w, int h)
{
	GLfloat aspect = (GLfloat)w / (GLfloat)h;
	m_projection = glm::perspective(glm::radians(45.0f), aspect, 1.0f, 40.0f);
}

void MyRender::initializeGL()
{
	initializeShader();
	initializeModel();
	initializeLight();

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
}

void MyRender::initializeShader()
{
	if (!m_modelProgram.addShaderFile(ShaderProgram::Vertex, "../GLSL/vertex_glsl.vert"))
		std::cout << m_modelProgram.lastError();
	if (!m_modelProgram.addShaderFile(ShaderProgram::Fragment, "../GLSL/fragment_glsl.frag"))
		std::cout << m_modelProgram.lastError();

	if (!m_modelProgram.link())
		std::cout << m_modelProgram.lastError();

	m_modelProgram.use();
	m_modelProgram.setValue("shininess", 64.0f);

	//创建六个灯光着色器程序
	for (int i = 0; i < 6; i++)
	{
		if (!m_lampProgram[i].addShaderFile(ShaderProgram::Vertex, "../GLSL/lamp_vertex_glsl.vert"))
			std::cout << m_lampProgram[i].lastError();
		if (!m_lampProgram[i].addShaderFile(ShaderProgram::Fragment, "../GLSL/lamp_fragment_glsl.frag"))
			std::cout << m_lampProgram[i].lastError();

		if (!m_lampProgram[i].link())
			std::cout << m_lampProgram[i].lastError();

		m_lampProgram[i].use();
		m_lampProgram[i].setValue("lightColor", m_lightColor[i]);
	}
}

void MyRender::initializeModel()
{
	m_model = new Model("../nanosuit/nanosuit.obj");
}

void MyRender::initializeLight()
{
	glm::vec3 vertices[] =
	{
		{ glm::vec3(-0.75f, -0.75f, 0.75f)  },
		{ glm::vec3(0.75f, -0.75f, 0.75f)   },
		{ glm::vec3(-0.375f, 0.75f, 0.375f) },
		{ glm::vec3(0.375f, 0.75f, 0.375f)  },

		{ glm::vec3(0.75f, -0.75f, 0.75f)   },
		{ glm::vec3(0.75f, -0.75f, -0.75f)  },
		{ glm::vec3(0.375f, 0.75f, 0.375f)  },
		{ glm::vec3(0.375f, 0.75f, -0.375f) },

		{ glm::vec3(0.75f, -0.75f, -0.75f)  },
		{ glm::vec3(-0.75f, -0.75f, -0.75f) },
		{ glm::vec3(0.375f, 0.75f, -0.375f) },
		{ glm::vec3(-0.375f, 0.75f, -0.375f)},

		{ glm::vec3(-0.75f, -0.75f, -0.75f) },
		{ glm::vec3(-0.75f, -0.75f, 0.75f)  },
		{ glm::vec3(-0.375f, 0.75f, -0.375f)},
		{ glm::vec3(-0.375f, 0.75f, 0.375f) },

		{ glm::vec3(-0.375f, 0.75f, 0.375f) },
		{ glm::vec3(0.375f, 0.75f, 0.375f)  },
		{ glm::vec3(-0.375f, 0.75f, -0.375f)},
		{ glm::vec3(0.375f, 0.75f, -0.375f) },

		{ glm::vec3(0.75f, -0.75f, 0.75f)   },
		{ glm::vec3(-0.75f, -0.75f, 0.75f)  },
		{ glm::vec3(0.75f, -0.75f, -0.75f)  },
		{ glm::vec3(-0.75f, -0.75f, -0.75f) }
	};

	GLushort indices[] =
	{
		0, 1, 2, 3, 3,
		4, 4, 5, 6, 7, 7,
		8, 8, 9, 10, 11, 11,
		12, 12, 13, 14, 15, 15,
		16, 16, 17, 18, 19, 19,
		20, 20, 21, 22, 23
	};
	glGenVertexArrays(1, &m_lampVao);
	glBindVertexArray(m_lampVao);	

	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &m_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, sizeof(glm::vec3), (void *)0);
	glEnableVertexAttribArray(0);
}