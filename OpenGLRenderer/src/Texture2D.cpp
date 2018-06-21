#include "Texture2D.h"
#include "stb_image.h"
#include "Macros.h"

#include <iostream>

// todo: texture number (GL_TEXTUREx)
Texture2D::Texture2D(const char* filepath, bool shouldFlip, int textureNumber, TextureType type)
	: m_TextureNumber(textureNumber),
	m_TextureType(type)
{
	int width, height, nChannels;
	stbi_set_flip_vertically_on_load(shouldFlip);
	unsigned char* data = stbi_load(filepath, &width, &height, &nChannels, STBI_rgb);

	GlCall(glGenTextures(1, &Id));
	this->setActive();

	GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
	GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
	GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

	if (data)
	{
		GlCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data));
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
