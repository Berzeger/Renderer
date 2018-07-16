#include "Texture2D.h"
#include "stb_image.h"
#include "Macros.h"

#include <iostream>

Texture2D::Texture2D(const std::string& filepath, bool shouldFlip, int textureNumber, TextureType type)
	: m_TextureNumber(textureNumber),
	m_TextureType(type),
	m_Filepath(filepath)
{
	int width, height, nChannels;
	stbi_set_flip_vertically_on_load(shouldFlip);
	unsigned char* data = stbi_load(filepath.c_str(), &width, &height, &nChannels, 0);

	GlCall(glGenTextures(1, &Id));
	this->setActive();

	GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
	GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
	GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

	if (data)
	{
		GLenum format;

		switch (nChannels)
		{
		case 1:
			format = GL_RED;
			break;
		case 3:
			format = GL_RGB;
			break;
		case 4:
			format = GL_RGBA;
			break;
		}

		GlCall(glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data));
		GlCall(glGenerateMipmap(GL_TEXTURE_2D));
		stbi_image_free(data);
	}
	else
	{
		std::cout << "Could not load image " << filepath << "." << std::endl;
	}
}

void Texture2D::setActive() const
{
	GlCall(glActiveTexture(GL_TEXTURE0 + m_TextureNumber));
	GlCall(glBindTexture(GL_TEXTURE_2D, Id));
}

TextureType Texture2D::getType() const
{
	return m_TextureType;
}

const std::string& Texture2D::getFilepath() const
{
	return m_Filepath;
}

void Texture2D::setTextureNumber(int textureNumber)
{
	m_TextureNumber = textureNumber;
}
