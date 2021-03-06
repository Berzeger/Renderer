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

		if (mesh->mNormals != nullptr)
		{
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.Normal = vector;
		}

		if (mesh->mTangents != nullptr)
		{
			vector.x = mesh->mTangents[i].x;
			vector.y = mesh->mTangents[i].y;
			vector.z = mesh->mTangents[i].z;
			vertex.Tangent = vector;
		}

		if (mesh->mBitangents != nullptr)
		{
			vector.x = mesh->mBitangents[i].x;
			vector.y = mesh->mBitangents[i].y;
			vector.z = mesh->mBitangents[i].z;
			vertex.Bitangent = vector;
		}

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

		std::vector<Texture2DPtr> heightMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, HEIGHT);
		textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

		std::vector<Texture2DPtr> ambientMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, AMBIENT);
		textures.insert(textures.end(), ambientMaps.begin(), ambientMaps.end());
	}

	return std::make_unique<Mesh>(vertices, indices, textures);
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
			std::string filename = m_Directory + '/' + str.C_Str();
			if (m_LoadedTextures[j]->getFilepath() == filename)
			{
				textures.push_back(m_LoadedTextures[j]);
				skip = true;
				break;
			}
		}

		if (!skip)
		{
			std::string filename = m_Directory + '/' + str.C_Str();
			Texture2DPtr ptr = std::make_shared<Texture2D>(filename, false, i, textureType);
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
