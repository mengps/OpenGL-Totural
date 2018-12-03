#ifndef MODEL_H
#define MODEL_H
#include "Mesh.h"
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using std::string;
class Model
{
public:
	explicit Model(const string &path);
	~Model();

	void render(ShaderProgram *program);

private:
	void loadModel(const string &path);
	void processNode(aiNode *node, const aiScene *scene);
	Mesh processMesh(aiMesh *mesh, const aiScene *scene);
	vector<Texture> loadMaterialTextures(aiMaterial *material, aiTextureType type, const string &name);
	unsigned int loadTexture(const string &filename);

	string m_directory;
	vector<Mesh> m_meshes;
	vector<Texture> m_loadedTextures;
};

#endif