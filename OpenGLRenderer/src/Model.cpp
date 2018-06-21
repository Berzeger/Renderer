#include "Model.h"

void Model::processNode(const aiNode * node, const aiScene * scene) const
{
	for (unsigned int i = 0; i < node->mNumMeshes; ++i)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		//m_Meshes.push_back(processMesh(mesh, scene));
	}

	for (unsigned int i = 0; i < node->mNumChildren; ++i)
	{
		processNode(node->mChildren[i], scene);
	}
}

MeshPtr Model::processMesh(const aiMesh * mesh, const aiScene * scene) const
{

}

void Model::loadModel(const std::string & path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "Error loading model " << path << std::endl;
		return;
	}

	m_Directory = path.substr(0, path.find_last_of('/'));
	processNode(scene->mRootNode, scene);
}

Model::Model(const std::string & path)
{
	loadModel(path);
}

void Model::draw(const Shader & shader)
{
	unsigned int size = m_Meshes.size();

	for (unsigned int i = 0; i < size; ++i)
	{
		m_Meshes[i].get()->draw(shader);
	}
}
