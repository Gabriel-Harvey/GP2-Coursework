#pragma once
#include <string>
#include <GL\glew.h>

class Texture
{
public:
	Texture(const std::string& fileName); //Stores and saves image data to texture. 

	void BindTexture(unsigned int unit); //Binds the texture to the model matrix.

	~Texture(); //Deletes the textures by number and address.

protected:
private:

	GLuint textHandler;
};

