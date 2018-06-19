#include "VertexBuffer.h"
#include "macros.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
	GlCall(glGenBuffers(1, &m_RendererId));
	this->bind();
	GlCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &m_RendererId);
}

void VertexBuffer::bind()
{
	GlCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererId));
}

void VertexBuffer::unbind()
{
	GlCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
