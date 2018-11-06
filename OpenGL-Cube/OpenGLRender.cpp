#include "OpenGLRender.h"
#include <iostream>
#include <fstream>
#include <sstream>

OpenGLRender::OpenGLRender()
{

}

OpenGLRender::~OpenGLRender()
{

}

GLuint OpenGLRender::compileShader(ShaderType type, const string &source)
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
				std::cerr << "Compile Vertex Shader Error :" << infoLog << std::endl;
			else if (type == Fragment)
				std::cerr << "Compile Fragment Shader Error :" << infoLog << std::endl;
			return 0;
		}
		return shader;
	}
	
	return 0;
}

GLuint OpenGLRender::compileShaderFile(ShaderType type, const string &filename)
{
	std::ifstream fin;
	fin.open(filename, std::ios_base::in);
	if (!fin.is_open())
		std::cerr << "Shader File " + filename + " Open Failed! " << std::endl;
	std::stringstream buffer;
	buffer << fin.rdbuf();
	string source(buffer.str());
	if (source.empty())
		std::cerr << "Shader File " + filename + " is Empty! " << std::endl;
	fin.close();

	return compileShader(type, source);
}
