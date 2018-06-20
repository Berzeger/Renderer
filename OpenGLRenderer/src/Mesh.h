#pragma once

#include <GLM/glm.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <memory>
#include <vector>

#include "Macros.h"
#include "Shader.h"
#include "Texture2D.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

typedef std::unique_ptr<Texture2D> Texture2DPtr;

class Mesh
{
private:
	std::vector<Vertex> m_Vertices;
	std::vector<unsigned int> m_Indices;
	std::vector<Texture2DPtr> m_Textures;
	VertexBuffer* m_VBO;
	IndexBuffer* m_IBO;
	unsigned int m_VAO;
	void setupMesh();

public:
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture2DPtr> textures);
	void draw(const Shader& shader) const;
	const std::vector<Vertex>& getVertices() const;
	const std::vector<unsigned int>& getIndices() const;
	const std::vector<Texture2DPtr>& getTextures() const;
};
