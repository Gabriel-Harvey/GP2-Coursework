#include "Mesh.h"
#include <vector>

//Initialises the models texture cooridnates and position.
void Mesh::initialize(Vertex* vertices, unsigned int verticesNum, unsigned int* indices, unsigned int indicesNum)
{
	IndexedModel IndModel;

	for (unsigned int i = 0; i < verticesNum; i++)
	{
		IndModel.pos.push_back(*vertices[i].getPosition());
		IndModel.textureCoords.push_back(*vertices[i].getTextCoordinate());
	}

	for (unsigned int i = 0; i < indicesNum; i++)
		IndModel.indices.push_back(indices[i]);

	initializeModel(IndModel);
}

//Creates the buffers for the models and stores them on the GPU.
void Mesh::initializeModel(const IndexedModel& IndModel)
{
	drawAmount = IndModel.indices.size();

	
	glGenVertexArrays(1, &vAO);//Creates a vertex array and stores it on the vertex array object(VAO).
	glBindVertexArray(vAO); //Binds the VAO.

	glGenBuffers(Number_BUFFERS, vAB); //Generates buffers using an array of data/buffers.

	glBindBuffer(GL_ARRAY_BUFFER, vAB[0]);//What type of data it is.
	glBufferData(GL_ARRAY_BUFFER, IndModel.pos.size() * sizeof(IndModel.pos[0]), &IndModel.pos[0], GL_STATIC_DRAW); //Moves the data to the GPU.
	glEnableVertexAttribArray(0); //Enables vertex attibute array 0.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, vAB[VB_TextCoord]);//What type of data it is.
	glBufferData(GL_ARRAY_BUFFER, IndModel.textureCoords.size() * sizeof(IndModel.textureCoords[0]), &IndModel.textureCoords[0], GL_STATIC_DRAW);//Moves the data to the GPU.
	glEnableVertexAttribArray(1);//Enables vertex attibute array 1.
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, vAB[VB_Normal]);//What type of data it is.
	glBufferData(GL_ARRAY_BUFFER, sizeof(IndModel.normals[0]) * IndModel.normals.size(), &IndModel.normals[0], GL_STATIC_DRAW);//Moves the data to the GPU.
	glEnableVertexAttribArray(2);//Enables vertex attibute array 2.
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vAB[VB_INDEX]);//What type of data it is.
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndModel.indices.size() * sizeof(IndModel.indices[0]), &IndModel.indices[0], GL_STATIC_DRAW);//Moves the data to the GPU.

	glBindVertexArray(0);
}

//Constructor.
Mesh::Mesh()
{
	drawAmount = NULL; //Sets draw amount to null.
}

//Loads the given model.
void Mesh::loadModel(const std::string& filename)
{
	IndexedModel indModel = OBJ_Model(filename).ToIndModel();
	initializeModel(indModel);
}

//Destructor deletes VAO.
Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &vAO);
}

//Draws the mesh to the screen.
void Mesh::drawMesh()
{
	glBindVertexArray(vAO); //Binds vertex array VAO.

	glDrawElements(GL_TRIANGLES, drawAmount, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

//Keeps the Collision Sphere bound to the mesh data.
void Mesh::updateSphereData(glm::vec3 pos, float radius)
{
	mSphere.SetSpherePosition(pos); //Updates the position of the collision sphere.
	mSphere.SetSphereRadius(radius); //Updates the radius of the collision sphere.
}