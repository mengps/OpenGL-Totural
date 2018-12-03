#include "ShaderProgram.h"
#include <fstream>
#include <sstream>

ShaderProgram::ShaderProgram()
{
	m_programId = glCreateProgram();
}

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(m_programId);
}

bool ShaderProgram::addShader(ShaderType type, const string &source)
{
	GLuint shader = compileShader(type, source);
	if (shader == 0)
		return false;
	else
	{
		glAttachShader(m_programId, shader);
		glDeleteShader(shader);
		return true;
	}
}

bool ShaderProgram::addShaderFile(ShaderType type, const string &filename)
{
	GLuint shader = compileShaderFile(type, filename);
	if (shader == 0)
		return false;
	else
	{
		glAttachShader(m_programId, shader);
		glDeleteShader(shader);
		return true;
	}
}

void ShaderProgram::use()
{
	glUseProgram(m_programId);
}

bool ShaderProgram::link()
{
	glLinkProgram(m_programId);

	GLint linked;
	GLchar infoLog[512];
	glGetProgramiv(m_programId, GL_LINK_STATUS, &linked);
	if (!linked)
	{
		glGetProgramInfoLog(m_programId, sizeof(infoLog), nullptr, infoLog);
		m_lastError = "Shader Program Linking Error :" + string(infoLog);
		return false;
	}
	else return true;
}

void ShaderProgram::setValue(const string &name, int value)
{
	glUniform1i(glGetUniformLocation(m_programId, name.c_str()), value);
}

void ShaderProgram::setValue(const string &name, float value)
{
	glUniform1f(glGetUniformLocation(m_programId, name.c_str()), value);
}

void ShaderProgram::setValue(const string &name, const glm::vec2 &value)
{
	glUniform2fv(glGetUniformLocation(m_programId, name.c_str()), 1, &value[0]);
}

void ShaderProgram::setValue(const string &name, const glm::vec3 &value)
{
	glUniform3fv(glGetUniformLocation(m_programId, name.c_str()), 1, &value[0]);
}

void ShaderProgram::setValue(const string &name, const glm::vec4 &value)
{
	glUniform4fv(glGetUniformLocation(m_programId, name.c_str()), 1, &value[0]);
}

void ShaderProgram::setValue(const string &name, const glm::mat2 &value)
{
	glUniformMatrix2fv(glGetUniformLocation(m_programId, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

void ShaderProgram::setValue(const string &name, const glm::mat3 &value)
{
	glUniformMatrix3fv(glGetUniformLocation(m_programId, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

void ShaderProgram::setValue(const string &name, const glm::mat4 &value)
{
	glUniformMatrix4fv(glGetUniformLocation(m_programId, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

GLuint ShaderProgram::compileShader(ShaderType type, const string &source)
{
	if (!source.empty())
	{
		GLuint shader = glCreateShader((GLenum)type);
		const GLchar *shaderSource = source.c_str();
		glShaderSource(shader, 1, &shaderSource, nullptr);
		glCompileShader(shader);

		GLint success;
		GLchar infoLog[512];
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, sizeof(infoLog), nullptr, infoLog);
			if (type == Vertex)
				m_lastError = "Compile Vertex Shader Error :" + string(infoLog);
			else if (type == Fragment)
				m_lastError = "Compile Fragment Shader Error :" + string(infoLog);
			else if (type == Geometry)
				m_lastError = "Compile Geometry Shader Error :" + string(infoLog);
		}
		return shader;
	}

	return 0;
}

GLuint ShaderProgram::compileShaderFile(ShaderType type, const string &filename)
{
	std::ifstream fin;
	fin.open(filename, std::ios_base::in);
	if (!fin.is_open())
		m_lastError = "Shader File " + filename + " Open Failed!";
	std::stringstream buffer;
	buffer << fin.rdbuf();
	string source(buffer.str());
	if (source.empty())
		m_lastError = "Shader File " + filename + " is Empty!";
	fin.close();

	return compileShader(type, source);
}