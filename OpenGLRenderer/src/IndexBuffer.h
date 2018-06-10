#pragma once
class IndexBuffer
{
private:
	unsigned int m_RendererId;
public:
	IndexBuffer(const void* data, unsigned int size);
	~IndexBuffer();

	void bind();
	void unbind();
};
