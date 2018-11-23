#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H
#include <string>
#include <glad/glad.h>
#include <glm/matrix.hpp>

using std::string;
class ShaderProgram
{
public:
	enum ShaderType
	{
		Vertex = GL_VERTEX_SHADER,
		Fragment = GL_FRAGMENT_SHADER,
		Geometry = GL_GEOMETRY_SHADER
	};

public:
	ShaderProgram();
	~ShaderProgram();

	GLuint programId() const { return m_programId; }
	string lastError() const { return m_lastError; }

	bool addShader(ShaderType type, const string &source);
	bool addShaderFile(ShaderType type, const string &filename);

	void use();
	bool link();
	void setValue(const string &name, int value);
	void setValue(const string &name, float value);
	void setValue(const string &name, const glm::vec2 &value);
	void setValue(const string &name, const glm::vec3 &value);
	void setValue(const string &name, const glm::vec4 &value);
	void setValue(const string &name, const glm::mat2 &value);
	void setValue(const string &name, const glm::mat3 &value);
	void setValue(const string &name, const glm::mat4 &value);

private:
	GLuint compileShader(ShaderType type, const string &source);
	GLuint compileShaderFile(ShaderType type, const string &filename);

private:
	string m_lastError;
	GLuint m_programId;
};

#endif