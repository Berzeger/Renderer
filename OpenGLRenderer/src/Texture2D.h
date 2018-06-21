#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <memory>

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
	const char* m_Filepath;

public:
	unsigned int Id;

	Texture2D(const char* filepath, bool shouldFlipVertically, int textureNumber, TextureType type);
	void setActive() const;
	void setTextureNumber(int textureNumber);
	TextureType getType() const;
	const char* getFilepath() const;
};

typedef std::shared_ptr<Texture2D> Texture2DPtr;
