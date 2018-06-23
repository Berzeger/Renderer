#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <memory>
#include <string>

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
	const std::string m_Filepath;

public:
	unsigned int Id;

	Texture2D(const std::string& filepath, bool shouldFlipVertically, int textureNumber, TextureType type);
	void setActive() const;
	void setTextureNumber(int textureNumber);
	TextureType getType() const;
	const std::string& getFilepath() const;
};

typedef std::shared_ptr<Texture2D> Texture2DPtr;
