#include "Mesh.h"
#include <vector>

//Initialises the models texture cooridnates.
void Mesh::initialise(Vertex* vertices, unsigned int verticesNum, unsigned int* indices, unsigned int indicesNum)
{
	IndexedModel IndModel;

	for (unsigned int i = 0; i < verticesNum; i++)
	{
		IndModel.pos.push_back(*vertices[i].getPosition());
		IndModel.textureCoords.push_back(*vertices[i].getTextCoordinate());
	}

	for (unsigned int i = 0; i < indicesNum; i++)
		IndModel.indices.push_back(indices[i]);

	initialiseModel(IndModel);
}

//Creates the buffers for the models and stores them on the GPU.
void Mesh::initialiseModel(const IndexedModel& IndModel)
{

	drawAmount = IndModel.indices.size();

	//GPU Bufffer
	glGenVertexArrays(1, &vAO);
	glBindVertexArray(vAO);

	glGenBuffers(Number_BUFFERS, vAB);

	glBindBuffer(GL_ARRAY_BUFFER, vAB[0]);
	glBufferData(GL_ARRAY_BUFFER, IndModel.pos.size() * sizeof(IndModel.pos[0]), &IndModel.pos[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, vAB[VB_TextCoord]);
	glBufferData(GL_ARRAY_BUFFER, IndModel.textureCoords.size() * sizeof(IndModel.textureCoords[0]), &IndModel.textureCoords[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, vAB[VB_Normal]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(IndModel.normals[0]) * IndModel.normals.size(), &IndModel.normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vAB[VB_INDEX]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndModel.indices.size() * sizeof(IndModel.indices[0]), &IndModel.indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
}

//Main Constructor.
Mesh::Mesh()
{
	drawAmount = NULL;
}

//Loads the given model.
void Mesh::loadModel(const std::string& filename)
{
	IndexedModel indModel = OBJ_Model(filename).ToIndModel();
	initialiseModel(indModel);
}

//Destructor.
Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &vAO);
}

//Draws the mesh to the screen.
void Mesh::drawMesh()
{
	glBindVertexArray(vAO);

	glDrawElements(GL_TRIANGLES, drawAmount, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

//Keeps the Collision Sphere bound to the position of the mesh.
void Mesh::updateSphereData(glm::vec3 pos, float radius)
{
	mSphere.SetSpherePosition(pos);
	mSphere.SetSphereRadius(radius);
}