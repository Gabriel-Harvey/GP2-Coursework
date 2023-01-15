#include "Texture.h"
#include "stb_image.h"
#include <cassert>
#include <iostream>

Texture::Texture(const std::string& fileName)
{
	//Width, heigh and number of componenets of the image supplied.
	int width, height, numComponents;
	unsigned char* imageData = stbi_load((fileName).c_str(), &width, &height, &numComponents, 4); //Loads the image.

	//Checks for errors.
	if (imageData == NULL)
	{
		std::cerr << "texture load failed" << fileName << std::endl;
	}

	//The adress and number of the texture.
	glGenTextures(1, &textHandler);
	glBindTexture(GL_TEXTURE_2D, textHandler);

	//Wraps the texture by width and height.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//Maginifies or shrinks the image if the area is too big or small.
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Specifies the texture with: Target, Mipmapping Level, Pixel Format, Width, Height, Border Size, Input Format, Data Type of Texturea and Image Data.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);

	stbi_image_free(imageData);
}

//Deletes the textures by number and address.
Texture::~Texture()
{
	glDeleteTextures(1, &textHandler);
}

//Binds the texture to the model matrix.
void Texture::BindTexture(unsigned int unit)
{
	assert(unit >= 0 && unit <= 31);

	//Sets the texture to be the active texture. 
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, textHandler);
}