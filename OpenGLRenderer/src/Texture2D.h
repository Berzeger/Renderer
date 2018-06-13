#pragma once

#include <GL\glew.h>
#include <GLFW\glfw3.h>

class Texture2D
{
private:
	GLenum m_TextureNumber;

public:
	unsigned int Id;

	Texture2D(const char* filepath, bool shouldFlipVertically, GLenum textureNumber);
	void setActive() const;
};