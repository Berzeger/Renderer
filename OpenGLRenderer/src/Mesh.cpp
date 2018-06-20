#include "Mesh.h"

void Mesh::setupMesh()
{
	GlCall(glGenVertexArrays(1, &m_VAO));
	GlCall(glBindVertexArray(m_VAO));

	// vertex positions
	GlCall(glEnableVertexAttribArray(0));
	GlCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0));

	// vertex normals
	GlCall(glEnableVertexAttribArray(1));
	GlCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal)));

	// vertex texture coords
	GlCall(glEnableVertexAttribArray(2));
	GlCall(glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords)));

	GlCall(glBindVertexArray(0));
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture2DPtr> textures)
	: m_Vertices(std::move(vertices)),
	m_Indices(std::move(indices)),
	m_Textures(std::move(textures))
{
	m_VBO = new VertexBuffer(&m_Vertices[0], m_Vertices.size() * sizeof(Vertex));
	m_IBO = new IndexBuffer(&m_Indices[0], m_Indices.size() * sizeof(unsigned int));
	setupMesh();
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
