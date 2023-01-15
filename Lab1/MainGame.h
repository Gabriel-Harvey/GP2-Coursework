#pragma once
#include <SDL\SDL.h>
#include <GL/glew.h>
#include "Display.h" 
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include "Transform.h"
#include "GameAudio.h"

enum class GameState { PLAY, EXIT };

class MainGame
{
public:
	MainGame();
	~MainGame();

	void run();

private:

	void initiliseSystems();
	void processGameInput();
	void runGameLoop();
	void drawGameWindow();
	bool objectCollision(glm::vec3 obj1Pos, float obj1Rad, glm::vec3 obj2Pos, float obj2Rad);
	
	Display _Display;
	GameState _currentGameState;
	Mesh mesh1;
	Mesh mesh2;
	Mesh mesh3;
	Camera mainCamera;
	Shader shader;
	GameAudio audio;

	SDL_Event event;

	float counter;
	float x = -15;
	float y = 0.0f;

	//Checks for if the movement keys are being pressed. 
	bool key_left = false;
	bool key_right = false;
	bool key_up = false;
	bool key_down = false;

	unsigned int bg;

	glm::vec3 position;

};

