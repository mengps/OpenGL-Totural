#include "MyRender.h"
#include "Image.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//使用glm::toMat4
#ifndef GLM_ENABLE_EXPERIMENTAL
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#endif

struct VertexData
{
	glm::vec3 postion;
	glm::vec3 color;
	glm::vec2 texCoord;
};

MyRender::MyRender()
{
	initializeGL();
}

MyRender::~MyRender()
{
	glDeleteTextures(1, &m_texture1);
	glDeleteTextures(1, &m_texture2);
	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_ebo);
	glDeleteProgram(m_program);
}

void MyRender::render(const glm::quat &rotation)
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	/* //标准示例
	glm::mat4 modelMatrix(1.0f);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, -5.0f));
	modelMatrix *= glm::toMat4(rotation);
	
	glUseProgram(m_program);
	GLuint mvp = glGetUniformLocation(m_program, "mvp");
	glUniformMatrix4fv(mvp, 1, GL_FALSE, glm::value_ptr(m_projection * modelMatrix));

	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLE_STRIP, 34, GL_UNSIGNED_SHORT, nullptr);
	*/
	
	glUseProgram(m_program);
	GLuint mvp = glGetUniformLocation(m_program, "mvp");

	for (int i = 0; i < 9; i++)
	{
		int row = i / 3;
		int column = i % 3;

		//z = -10.0f
		{
			glm::mat4 modelMatrix(1.0f);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(-3.0f + column * 3.0f, -3.0f + row * 3.0f, -10.0f));
			modelMatrix *= glm::toMat4(rotation);

			glUniformMatrix4fv(mvp, 1, GL_FALSE, glm::value_ptr(m_projection * modelMatrix));
			glBindVertexArray(m_vao);
			glDrawElements(GL_TRIANGLE_STRIP, 34, GL_UNSIGNED_SHORT, nullptr);
		}
		//z = -14.0f
		{
			glm::mat4 modelMatrix(1.0f);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(-3.0f + column * 3.0f, -3.0f + row * 3.0f, -14.0f));
			modelMatrix *= glm::toMat4(rotation);

			glUniformMatrix4fv(mvp, 1, GL_FALSE, glm::value_ptr(m_projection * modelMatrix));
			glBindVertexArray(m_vao);
			glDrawElements(GL_TRIANGLE_STRIP, 34, GL_UNSIGNED_SHORT, nullptr);
		}
		//z = -18.0f
		{
			glm::mat4 modelMatrix(1.0f);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(-3.0f + column * 3.0f, -3.0f + row * 3.0f, -18.0f));
			modelMatrix *= glm::toMat4(rotation);

			glUniformMatrix4fv(mvp, 1, GL_FALSE, glm::value_ptr(m_projection * modelMatrix));
			glBindVertexArray(m_vao);
			glDrawElements(GL_TRIANGLE_STRIP, 34, GL_UNSIGNED_SHORT, nullptr);
		}
		//z = -22.0f
		{
			glm::mat4 modelMatrix(1.0f);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(-3.0f + column * 3.0f, -3.0f + row * 3.0f, -22.0f));
			modelMatrix *= glm::toMat4(rotation);

			glUniformMatrix4fv(mvp, 1, GL_FALSE, glm::value_ptr(m_projection * modelMatrix));
			glBindVertexArray(m_vao);
			glDrawElements(GL_TRIANGLE_STRIP, 34, GL_UNSIGNED_SHORT, nullptr);
		}
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
	initializeTexture();
	initializeCube();

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
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

	glUseProgram(m_program);
	glUniform1i(glGetUniformLocation(m_program, "texture1"), 0);
	glUniform1i(glGetUniformLocation(m_program, "texture2"), 1);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void MyRender::initializeTexture()
{
	glGenTextures(1, &m_texture1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	Image image1("../container.jpg");
	if (image1.data())
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image1.width(), image1.height(),
			0, GL_RGB, GL_UNSIGNED_BYTE, image1.data());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else std::cout << "Open Image1 Error! ";

	glGenTextures(1, &m_texture2);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_texture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	Image image2("../face.jpg", true);
	if (image2.data())
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image2.width(), image2.height(),
			0, GL_RGB, GL_UNSIGNED_BYTE, image2.data());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else std::cout << "Open Image2 Error! ";
}

void MyRender::initializeCube()
{
	VertexData vertices[] =
	{
		{ glm::vec3( -0.75f, -0.75f,  0.75f), glm::vec3(0.8f, 0.8f, 0.0f), glm::vec2( 0.0f, 0.0f) },
		{ glm::vec3(  0.75f, -0.75f,  0.75f), glm::vec3(0.0f, 0.8f, 0.8f), glm::vec2( 1.0f, 0.0f) },
		{ glm::vec3(-0.375f,  0.75f, 0.375f), glm::vec3(0.0f, 0.8f, 0.8f), glm::vec2(0.33f, 1.0f) },
		{ glm::vec3( 0.375f,  0.75f, 0.375f), glm::vec3(0.8f, 0.0f, 0.0f), glm::vec2(0.66f, 1.0f) },

		{ glm::vec3( 0.75f, -0.75f,   0.75f), glm::vec3(0.8f, 0.8f, 0.0f), glm::vec2( 0.0f, 0.0f) },
		{ glm::vec3( 0.75f, -0.75f,  -0.75f), glm::vec3(0.0f, 0.8f, 0.8f), glm::vec2( 1.0f, 0.0f) },
		{ glm::vec3(0.375f,  0.75f,  0.375f), glm::vec3(0.0f, 0.8f, 0.8f), glm::vec2(0.33f, 1.0f) },
		{ glm::vec3(0.375f,  0.75f, -0.375f), glm::vec3(0.8f, 0.0f, 0.0f), glm::vec2(0.66f, 1.0f) },

		{ glm::vec3(  0.75f, -0.75f,  -0.75f), glm::vec3(0.8f, 0.8f, 0.0f), glm::vec2( 0.0f, 0.0f) },
		{ glm::vec3( -0.75f, -0.75f,  -0.75f), glm::vec3(0.0f, 0.8f, 0.8f), glm::vec2( 1.0f, 0.0f) },
		{ glm::vec3( 0.375f,  0.75f, -0.375f), glm::vec3(0.0f, 0.8f, 0.8f), glm::vec2(0.33f, 1.0f) },
		{ glm::vec3(-0.375f,  0.75f, -0.375f), glm::vec3(0.8f, 0.0f, 0.0f), glm::vec2(0.66f, 1.0f) },

		{ glm::vec3( -0.75f, -0.75f,  -0.75f), glm::vec3(0.8f, 0.8f, 0.0f), glm::vec2( 0.0f, 0.0f) },
		{ glm::vec3( -0.75f, -0.75f,   0.75f), glm::vec3(0.0f, 0.8f, 0.8f), glm::vec2( 1.0f, 0.0f) },
		{ glm::vec3(-0.375f,  0.75f, -0.375f), glm::vec3(0.0f, 0.8f, 0.8f), glm::vec2(0.33f, 1.0f) },
		{ glm::vec3(-0.375f,  0.75f,  0.375f), glm::vec3(0.8f, 0.0f, 0.0f), glm::vec2(0.66f, 1.0f) },

		{ glm::vec3(-0.375f, 0.75f,  0.375f), glm::vec3(0.0f, 0.8f, 0.8f), glm::vec2(0.0f, 0.0f) },
		{ glm::vec3( 0.375f, 0.75f,  0.375f), glm::vec3(0.8f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f) },
		{ glm::vec3(-0.375f, 0.75f, -0.375f), glm::vec3(0.0f, 0.8f, 0.8f), glm::vec2(0.0f, 1.0f) },
		{ glm::vec3( 0.375f, 0.75f, -0.375f), glm::vec3(0.8f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f) },

		{ glm::vec3( 0.75f, -0.75f,  0.75f), glm::vec3(0.8f, 0.8f, 0.0f), glm::vec2(0.0f, 0.0f) },
		{ glm::vec3(-0.75f, -0.75f,  0.75f), glm::vec3(0.0f, 0.8f, 0.8f), glm::vec2(1.0f, 0.0f) },
		{ glm::vec3( 0.75f, -0.75f, -0.75f), glm::vec3(0.8f, 0.8f, 0.0f), glm::vec2(0.0f, 1.0f) },
		{ glm::vec3(-0.75f, -0.75f, -0.75f), glm::vec3(0.0f, 0.8f, 0.8f), glm::vec2(1.0f, 1.0f) }
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

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

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
	glVertexAttribPointer(location + 2, 2, GL_FLOAT, GL_TRUE, sizeof(VertexData), (void *)(sizeof(glm::vec3) * 2));
	glEnableVertexAttribArray(location + 2);
}