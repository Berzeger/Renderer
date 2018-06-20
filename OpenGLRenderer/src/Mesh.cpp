#include "Mesh.h"

void Mesh::setupMesh()
{
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture2DPtr> textures)
	: m_Vertices(std::move(vertices)),
	m_Indices(std::move(indices)),
	m_Textures(std::move(textures))
{
}

void Mesh::draw(const Shader & shader) const
{
}

const std::vector<Vertex>& Mesh::getVertices() const
{
	return m_Vertices;
}

const std::vector<unsigned int>& Mesh::getIndices() const
{
	return m_Indices;
}

const std::vector<Texture2DPtr>& Mesh::getTextures() const
{
	return m_Textures;
}
