#include "IndexBuffer.h"
#include "macros.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

IndexBuffer::IndexBuffer(const void* data, unsigned int size)
{
	GlCall(glGenBuffers(1, &m_RendererId));
	GlCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererId));
	GlCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &m_RendererId);
}

void IndexBuffer::bind()
{
	GlCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererId));
}

void IndexBuffer::unbind()
{
	GlCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
