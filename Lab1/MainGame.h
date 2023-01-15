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
	MainGame(); //Creates pointers and sets the game state.
	~MainGame(); //Destructor.

	void run();  //Calls the constructors for initailising the program and starting the gameloop.

private:

	void initiliseSystems(); //Initilises all component parts of the program.
	void processGameInput(); //Processes all game and user input.
	void runGameLoop(); //While game is active, will loop all game components.
	void drawGameWindow(); //Draws the meshes to the display and shows translations.
	bool objectCollision(glm::vec3 obj1Pos, float obj1Rad, glm::vec3 obj2Pos, float obj2Rad, string names); //Manges collisions between meshes and updates the console.
	
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
	float x = -15; //Moves the dog mesh in X direction.
	float y = 0.0f; //Moves the dog mesh in Y direction.

	//Checks for if the movement keys are being pressed. 
	bool key_left = false;
	bool key_right = false;
	bool key_up = false;
	bool key_down = false;

	unsigned int bg;

	glm::vec3 position;

};

