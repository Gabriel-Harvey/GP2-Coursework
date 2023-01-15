#pragma once
#include <glm\glm.hpp>
#include <GL\glew.h>
#include <string>
#include "Obj_Loader.h"

struct Vertex
{
public:
	Vertex(const glm::vec3& position, const glm::vec2& texCoordinate)
	{
		this->position = position;
		this->texCoordinate = texCoordinate;
		this->normal = normal;
	}

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

	Sphere()
	{
	}

	glm::vec3 GetSpherePosition() { return position; }
	float GetSphereRadius() { return sphereRad; }

	void SetSpherePosition(glm::vec3 pos)
	{
		this->position = pos;
	}

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

	void drawMesh();
	void initialise(Vertex* vertices, unsigned int verticesNum, unsigned int* indices, unsigned int indicesNum);
	void loadModel(const std::string& filename);
	void initialiseModel(const IndexedModel& model);

	void updateSphereData(glm::vec3 pos, float radius);
	glm::vec3 getSpherePos() { return mSphere.GetSpherePosition(); }
	float getSphereRad() { return mSphere.GetSphereRadius(); }

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

