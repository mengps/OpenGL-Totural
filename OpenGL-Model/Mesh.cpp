#include "Mesh.h"
#include "ShaderProgram.h"
#include <glad/glad.h>
#include <iostream>

Mesh::Mesh(const vector<VertexData> &vertices,
		   const vector<unsigned int> &indices, 
		   const vector<Texture> &textures) :
	m_vertices(vertices),
	m_indices(indices),
	m_textures(textures)
{
	setupMesh();
}

Mesh::~Mesh()
{

}

void Mesh::render(ShaderProgram *program)
{
	size_t diffuseNum = 1;
	size_t specularNum = 1;
	size_t normalNum = 1;
	size_t heightNum = 1;
	size_t size = m_textures.size();

	for (size_t i = 0; i < size; i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, m_textures[i].id);
		string name = m_textures[i].name;
		if (name == "diffuse")
			name += std::to_string(diffuseNum++);
		else if (name == "specular")
			name += std::to_string(specularNum++);
		else if (name == "normal")
			name += std::to_string(normalNum++);
		else if (name == "height")
			name += std::to_string(heightNum++);

		program->setValue(("material." + name).c_str(), (int)i);
	}

	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
	glActiveTexture(GL_TEXTURE0);
}

void Mesh::setupMesh()
{
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ebo);

	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(VertexData), &m_vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), &m_indices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void *)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void *)offsetof(VertexData, normal));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void *)offsetof(VertexData, texCoord));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, tangent));
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, bitangent));

	glBindVertexArray(0);
}
