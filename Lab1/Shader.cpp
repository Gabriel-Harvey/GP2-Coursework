#include "Shader.h"
#include <iostream>
#include <fstream>

//Contructor.
Shader::Shader()
{
}

//Initialises the shader.
void Shader::initialize()
{
	shaderProgram = glCreateProgram(); //Creates a shader program.
	shaders[0] = NewShader(LoadExistingShader("..\\res\\shader.vert"), GL_VERTEX_SHADER); //creates a vertex shader.
	shaders[1] = NewShader(LoadExistingShader("..\\res\\shader.frag"), GL_FRAGMENT_SHADER); //creates a fragment shaer.

	//Adds the shaders to the program.
	for (unsigned int i = 0; i < NUM_SHADERS; i++)
	{
		glAttachShader(shaderProgram, shaders[i]);
	}

	glBindAttribLocation(shaderProgram, 0, "position");
	glBindAttribLocation(shaderProgram, 1, "texCoord");

	glLinkProgram(shaderProgram);
	CheckForShaderError(shaderProgram, GL_LINK_STATUS, true, "Error: Link to shader program has failed "); //Checks for error.

	glValidateProgram(shaderProgram);
	CheckForShaderError(shaderProgram, GL_VALIDATE_STATUS, true, "Error: Shader program is not valid"); // checks for error.

	uniforms[TRANSFORM_U] = glGetUniformLocation(shaderProgram, "transform");
}

//Detaches shaders for the program ans deletes the shader program.
Shader::~Shader()
{
	//Detaches each shader in the program.
	for (unsigned int i = 0; i < NUM_SHADERS; i++)
	{
		glDetachShader(shaderProgram, shaders[i]);
		glDeleteShader(shaders[i]);
	}
	glDeleteProgram(shaderProgram);
}

//Installs the program object specified by program as part of rendering state. 
void Shader::BindToGPU()
{
	glUseProgram(shaderProgram);
}

//Upates the shader program base on the camera.
void Shader::Update(const Transform& trans, const Camera& cam)
{
	glm::mat4 mvp = cam.GetGameViewProj() * trans.GrabModel();
	glUniformMatrix4fv(uniforms[TRANSFORM_U], 1, GLU_FALSE, &mvp[0][0]);
}

//Creates a new shader of given type. 
GLuint Shader::NewShader(const std::string& text, unsigned int type)
{
	GLuint shader = glCreateShader(type);

	//Checks for error when creating shader by type.
	if (shader == 0)
		std::cerr << "Error: shader type creation has failed " << type << std::endl;

	const GLchar* stringSource[1];
	stringSource[0] = text.c_str();
	GLint lengths[1];
	lengths[0] = text.length();

	glShaderSource(shader, 1, stringSource, lengths);
	glCompileShader(shader);

	CheckForShaderError(shader, GL_COMPILE_STATUS, false, "Error: compiling shader!"); //Checks for error.

	return shader;
}

//Loads an existing shader into the program.
std::string Shader::LoadExistingShader(const std::string& fileName)
{
	//Opens the file contaning the shader.
	std::ifstream file;
	file.open((fileName).c_str());

	std::string output;
	std::string line;

	//Checks for error.
	if (file.is_open())
	{
		while (file.good())
		{
			getline(file, line);
			output.append(line + "\n");
		}
	}
	else
	{
		std::cerr << "Unable to load shader: " << fileName << std::endl;
	}

	return output;
}

//Checks for errors in the shader program.
void Shader::CheckForShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage)
{
	GLint success = 0;
	GLchar error[1024] = { 0 };

	if (isProgram)
		glGetProgramiv(shader, flag, &success);
	else
		glGetShaderiv(shader, flag, &success);

	//Returns error message if it finds error.
	if (success == GL_FALSE)
	{
		if (isProgram)
			glGetProgramInfoLog(shader, sizeof(error), NULL, error);
		else
			glGetShaderInfoLog(shader, sizeof(error), NULL, error);

		std::cerr << errorMessage << ": '" << error << "'" << std::endl;
	}
}