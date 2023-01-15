#pragma once
#include <string>
#include <GL\glew.h>
#include "Transform.h"
#include "Camera.h"

class Shader
{
public:
	Shader();

	void BindToGPU();
	void Update(const Transform& transform, const Camera& cam);
	void initialize();

	std::string Shader::LoadExistingShader(const std::string& fileName);
	void Shader::CheckForShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);
	GLuint Shader::NewShader(const std::string& text, unsigned int type);

	~Shader();


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

