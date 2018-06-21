#pragma once
#include <string>
#include <memory>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Shader.h"
#include "Mesh.h"
#include "Texture2D.h"

class Model
{
private:
	std::vector<Texture2DPtr> m_LoadedTextures;
	std::vector<MeshPtr> m_Meshes;
	std::string m_Directory;

	void processNode(const aiNode* node, const aiScene* scene);
	MeshPtr processMesh(const aiMesh* mesh, const aiScene* scene);
	void loadModel(const std::string& path);
	std::vector<Texture2DPtr> loadMaterialTextures(const aiMaterial* mat, aiTextureType type, TextureType textureType);
public:
	Model(const std::string& path);
	void draw(const Shader& shader);
};
