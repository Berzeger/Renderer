#pragma once
#include <string>
#include <memory>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Shader.h"
#include "Mesh.h"

typedef std::unique_ptr<Mesh> MeshPtr;

class Model
{
private:
	std::vector<MeshPtr> m_Meshes;
	std::string m_Directory;

	void loadModel(const std::string& path);
public:
	Model(const std::string& path);
	void draw(const Shader& shader);
};

