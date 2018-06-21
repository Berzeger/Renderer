#include "Model.h"

void Model::loadModel(const std::string & path)
{
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
