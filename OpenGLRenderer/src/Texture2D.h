#pragma once

#include <GL\glew.h>
#include <GLFW\glfw3.h>

class Texture2D
{
private:
	int m_TextureNumber;

public:
	unsigned int Id;

	Texture2D(const char* filepath, bool shouldFlipVertically, int textureNumber);
	void setActive() const;
};