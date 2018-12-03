#include "Model.h"
#include "Image.h"
#include <iostream>
#include <glad/glad.h>

Model::Model(const string &path)
{
	loadModel(path);
}

Model::~Model()
{

}

void Model::render(ShaderProgram *program)
{
	for (auto it : m_meshes)
		it.render(program);
}

void Model::loadModel(const string &path)
{
	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals | aiProcess_CalcTangentSpace);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << importer.GetErrorString() << std::endl;
		return;
	}

	m_directory = path.substr(0, path.find_last_of("//"));
	processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, const aiScene *scene)
{
	for (size_t i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		m_meshes.push_back(processMesh(mesh, scene));
	}

	for (size_t i = 0; i < node->mNumChildren; i++)
		processNode(node->mChildren[i], scene);
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
	vector<VertexData> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;

	for (size_t i = 0; i < mesh->mNumVertices; i++)
	{
		VertexData vertexData;
		glm::vec3 postition;
		postition.x = mesh->mVertices[i].x;
		postition.y = mesh->mVertices[i].y;
		postition.z = mesh->mVertices[i].z;
		vertexData.position = postition;
		
		glm::vec3 normal;
		normal.x = mesh->mNormals[i].x;
		normal.y = mesh->mNormals[i].y;
		normal.z = mesh->mNormals[i].z;
		vertexData.normal = normal;

		if (mesh->mTextureCoords[0])
		{
			glm::vec2 texCoord;
			texCoord.x = mesh->mTextureCoords[0][i].x;
			texCoord.y = mesh->mTextureCoords[0][i].y;
			vertexData.texCoord = texCoord;
		}
		else vertexData.texCoord = glm::vec2(0.0f, 0.0f);

		/*glm::vec3 tangent;
		tangent.x = mesh->mTangents[i].x;
		tangent.y = mesh->mTangents[i].y;
		tangent.z = mesh->mTangents[i].z;
		vertexData.tangent = tangent;

		glm::vec3 bitangent;
		bitangent.x = mesh->mBitangents[i].x;
		bitangent.y = mesh->mBitangents[i].y;
		bitangent.z = mesh->mBitangents[i].z;
		vertexData.bitangent = bitangent;*/

		vertices.push_back(vertexData);
	}

	for (size_t i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (size_t j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	if (mesh->mMaterialIndex > 0)
	{
		aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
		vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}

	return Mesh(vertices, indices, textures);
}

vector<Texture> Model::loadMaterialTextures(aiMaterial *material, aiTextureType type, const string &name)
{
	vector<Texture> textures;
	for (size_t i = 0; i < material->GetTextureCount(type); i++)
	{
		bool loaded = false;
		aiString path;
		material->GetTexture(type, i, &path);

		for (auto it : m_loadedTextures)
		{
			//如果是已经载入过的纹理
			if (it.path == path.C_Str())
			{
				textures.push_back(it);
				loaded = true;
				break;
			}
		}
		if (!loaded)
		{
			Texture texture;
			texture.id = loadTexture(path.C_Str());
			texture.name = name;
			texture.path = path.C_Str();
			textures.push_back(texture);
			m_loadedTextures.push_back(texture);
		}
	}

	return textures;
}

unsigned int Model::loadTexture(const string &filename)
{
	Image texture(m_directory + '/' + filename);
	GLuint textureId;
	glGenTextures(1, &textureId);
	std::cout << "Directory :" << m_directory + '/' + filename << std::endl;
	if (texture.data())
	{
		GLenum format;
		if (texture.channels() == 1)
			format = GL_RED;
		else if (texture.channels() == 3)
			format = GL_RGB;
		else if (texture.channels() == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureId);
		glTexImage2D(GL_TEXTURE_2D, 0, format, texture.width(), texture.height(),
					 0, format, GL_UNSIGNED_BYTE, texture.data());
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else std::cout << "Open Texture " + filename + " Error!";

	return textureId;
}
