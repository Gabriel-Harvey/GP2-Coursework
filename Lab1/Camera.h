#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

struct Camera
{
public:
	Camera()
	{
	}

	//Initialises the camera varibles
	void initializeCamera(const glm::vec3& position, float fov, float aspect, float nearClip, float farClip)
	{
		this->position = position;
		this->forward = glm::vec3(0.0f, 0.0f, 1.0f);
		this->up = glm::vec3(0.0f, 1.0f, 0.0f);
		this->proj = glm::perspective(fov, aspect, nearClip, farClip);
	}

	//Grabs the view direction.
	inline glm::mat4 GetGameViewProj() const
	{
		return proj * glm::lookAt(position, position + forward, up);
	}

protected:
private:
	glm::mat4 proj;
	glm::vec3 position;
	glm::vec3 forward;
	glm::vec3 up;
};
