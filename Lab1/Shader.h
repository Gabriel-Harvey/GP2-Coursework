#pragma once
#include <string>
#include <GL\glew.h>
#include "Transform.h"
#include "Camera.h"

class Shader
{
public:
	Shader(); //Contructor.

	void BindToGPU(); //Installs the program object specified by program as part of rendering state. 
	void Update(const Transform& transform, const Camera& cam); //Upates the shader program base on the camera.
	void initialize(); //Initialises the shader.

	std::string Shader::LoadExistingShader(const std::string& fileName); //Loads an existing shader into the program.
	void Shader::CheckForShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage); //Checks for errors in the shader program.
	GLuint Shader::NewShader(const std::string& text, unsigned int type); //Creates a new shader of given type. 

	~Shader(); //Detaches shaders for the program ans deletes the shader program.


protected:
private:
	static const unsigned int NUM_SHADERS = 2;

	enum
	{
		TRANSFORM_U,
		NUM_UNI
	};

	GLuint shaderProgram;
	GLuint shaders[NUM_SHADERS];
	GLuint uniforms[NUM_UNI];
};

