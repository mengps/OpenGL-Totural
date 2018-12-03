#ifndef OPENGLMESH_H
#define OPENGLMESH_H
#include <string>
#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

using std::string;
using std::vector;
struct VertexData
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoord;
	glm::vec3 tangent;
	glm::vec3 bitangent;
};

struct Texture
{
	unsigned int id;
	string name;
	string path;
};

class ShaderProgram;
class Mesh
{
public:
	Mesh(const vector<VertexData> &vertices,
		 const vector<unsigned int> &indices, 
		 const vector<Texture> &textures);
	~Mesh();

	void render(ShaderProgram *program);

private:
	void setupMesh();

	vector<VertexData> m_vertices;
	vector<unsigned int> m_indices;
	vector<Texture> m_textures;

	unsigned int m_vao, m_vbo, m_ebo;
};

#endif