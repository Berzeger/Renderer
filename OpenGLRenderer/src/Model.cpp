#include "Model.h"

void Model::processNode(const aiNode * node, const aiScene * scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; ++i)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		m_Meshes.push_back(processMesh(mesh, scene));
	}

	for (unsigned int i = 0; i < node->mNumChildren; ++i)
	{
		processNode(node->mChildren[i], scene);
	}
}

MeshPtr Model::processMesh(const aiMesh * mesh, const aiScene * scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture2DPtr> textures;

	// Vertices
	for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
	{
		Vertex vertex;

		glm::vec3 vector;
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector;

		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.Normal = vector;

		// Does the mesh contain texture coordinates?
		if (mesh->mTextureCoords[0])
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		}
		else
		{
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);
		}

		vertices.push_back(vertex);
	}

	// Indices
	for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
	{
		aiFace face = mesh->mFaces[i];

		for (unsigned int j = 0; j < face.mNumIndices; ++j)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	// Material
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<Texture2DPtr> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, DIFFUSE);
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		std::vector<Texture2DPtr> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, SPECULAR);
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}

	MeshPtr rv(new Mesh(vertices, indices, textures));
	return rv;
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

std::vector<Texture2DPtr> Model::loadMaterialTextures(const aiMaterial * mat, aiTextureType type, TextureType textureType)
{
	std::vector<Texture2DPtr> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); ++i)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		bool skip = false;

		unsigned int texturesCount = m_LoadedTextures.size();
		for (unsigned int j = 0; j < texturesCount; ++j)
		{
			if (std::strcmp(m_LoadedTextures[j]->getFilepath(), str.C_Str()) == 0)
			{
				textures.push_back(m_LoadedTextures[j]);
				skip = true;
				break;
			}
		}

		if (!skip)
		{
			std::string filename = std::string(str.C_Str());
			filename = m_Directory + '/' + filename;
			Texture2D texture(filename.c_str(), false, i, textureType);
			Texture2DPtr ptr = std::make_shared<Texture2D>(texture);
			textures.push_back(ptr);
			m_LoadedTextures.push_back(ptr);
		}
	}

	return textures;
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
