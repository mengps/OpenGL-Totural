#ifndef OPENGLRENDER_H
#define OPENGLRENDER_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

using std::string;
class OpenGLRender
{
public:
	enum ShaderType
	{
		Vertex = GL_VERTEX_SHADER,
		Fragment = GL_FRAGMENT_SHADER
	};
	OpenGLRender();
	~OpenGLRender();

public:	
	virtual void render() { }
	virtual void resizeGL(int w, int h) { }
	virtual void initializeGL() { }
	virtual void initializeShader() { }

protected:
	GLuint compileShader(ShaderType type, const string &source);
	GLuint compileShaderFile(ShaderType type, const string &filename);
};

#endif