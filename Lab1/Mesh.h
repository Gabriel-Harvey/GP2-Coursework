#pragma once
#include <glm\glm.hpp>
#include <GL\glew.h>
#include <string>
#include "Obj_Loader.h"

struct Vertex
{
public:
	//Sets mesh/vertex data.
	Vertex(const glm::vec3& position, const glm::vec2& texCoordinate)
	{
		this->position = position;
		this->texCoordinate = texCoordinate;
		this->normal = normal;
	}

	//Grabs mesh/vertex data.
	glm::vec3* getPosition() { return &position; }
	glm::vec2* getTextCoordinate() { return &texCoordinate; }
	glm::vec3* GetNormal() { return &normal; }

private:

	glm::vec3 position;
	glm::vec2 texCoordinate;
	glm::vec3 normal;
};

struct Sphere
{
public:
	//Constructor.
	Sphere()
	{
	}

	glm::vec3 GetSpherePosition() { return position; } //Grabs sphere position.
	float GetSphereRadius() { return sphereRad; } //Grabs Sphere radius.

	//Sets sphere position to given vec3.
	void SetSpherePosition(glm::vec3 pos)
	{
		this->position = pos;
	}

	//Sets sphere radius to given float.
	void SetSphereRadius(float rad)
	{
		this->sphereRad = rad;
	}

private:
	glm::vec3 position;
	float sphereRad;
};


class Mesh
{
public:
	Mesh();
	~Mesh();

	void drawMesh(); //Draws the mesh to display.
	void initialize(Vertex* vertices, unsigned int verticesNum, unsigned int* indices, unsigned int indicesNum); //Initialises the models texture cooridnates and position.
	void loadModel(const std::string& filename); //Loads the given model.
	void initializeModel(const IndexedModel& model); //Creates the buffers for the models and stores them on the GPU.
	void updateSphereData(glm::vec3 pos, float radius); //Keeps the Collision Sphere bound to the mesh data.

	glm::vec3 getSpherePos() { return mSphere.GetSpherePosition(); } //Grabs sphere position.
	float getSphereRad() { return mSphere.GetSphereRadius(); } //Grabs sphere radius.

private:

	enum
	{
		VB_Position,
		VB_TextCoord,
		VB_Normal,
		VB_INDEX,
		Number_BUFFERS
	};

	Sphere mSphere;
	GLuint vAO;
	GLuint vAB[Number_BUFFERS];
	unsigned int drawAmount;
};

