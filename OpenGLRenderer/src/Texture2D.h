#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

enum TextureType
{
	DIFFUSE,
	SPECULAR,
	EMISSIVE,
	OTHER
};

class Texture2D
{
private:
	int m_TextureNumber;
	TextureType m_TextureType;

public:
	unsigned int Id;

	Texture2D(const char* filepath, bool shouldFlipVertically, int textureNumber, TextureType type);
	void setActive() const;
	TextureType getType() const;
};