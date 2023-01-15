#include "MainGame.h"
#include "Camera.h"
#include <iostream>
#include <string>


Transform transform;

MainGame::MainGame() //Creates pointers and sets the game state.
{
	_currentGameState = GameState::PLAY;
	Display* _display = new Display();
	Mesh* mesh1();
	Mesh* mesh2();
	Mesh* mesh3();
}

MainGame::~MainGame() //Destuctor
{
}

void MainGame::run() //Calls the constructors for initailising the program and starting the gameloop.
{
	initiliseSystems();
	runGameLoop();
}

void MainGame::initiliseSystems() //Initilises all component parts of the program.
{
	//Creates the display.
	_Display.initDisplay();

	//Loads the 3D models that are used in the program.
	mesh1.loadModel("..\\res\\monkey3.obj");
	mesh2.loadModel("..\\res\\apple.obj");
	mesh3.loadModel("..\\res\\Dog.obj");

	//Adds the background music as a track.
	audio.newAudioTrack("..\\res\\background.wav");

	shader.initialize();

	//Creates the camera with the displays widthand hieght.
	mainCamera.initializeCamera(glm::vec3(0, 0, -30), 70.0f, (float)_Display.getScreenWidth() / _Display.getScreenHeight(), 0.01f, 1000.0f);

	counter = 0.0f;
}

void MainGame::runGameLoop() //While game is active, will loop all game components.
{
	while (_currentGameState != GameState::EXIT)
	{
		audio.playAudioTrack();
		processGameInput();
		drawGameWindow();
		objectCollision(mesh1.getSpherePos(), mesh1.getSphereRad(), mesh2.getSpherePos(), mesh2.getSphereRad(), "Monkey & Apple");
		objectCollision(mesh2.getSpherePos(), mesh2.getSphereRad(), mesh3.getSpherePos(), mesh3.getSphereRad(), "Apple & Dog");
		objectCollision(mesh1.getSpherePos(), mesh1.getSphereRad(), mesh3.getSpherePos(), mesh3.getSphereRad(), "Dog & Monkey");
	}
}

void MainGame::processGameInput() //Processes all game and user input.
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
				_currentGameState = GameState::EXIT;
				break;
		
			//Tracks when keys are pressed down.
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) 
				{
					case SDLK_a:						
						key_left = true;
						break;

					case SDLK_d:						
						key_right = true;
						break;

					case SDLK_s:					
						key_down = true;
						break;

					case SDLK_w:					
						key_up = true;		
						break;
				}
				break;

			//Tracks when keys are lifted.
			case SDL_KEYUP:
				switch (event.key.keysym.sym) 
				{
					case SDLK_a:
						key_left = false;
						break;

					case SDLK_d:
						key_right = false;
						break;

					case SDLK_s:
						key_down = false;
						break;

					case SDLK_w:
						key_up = false;
						break;
				}
				break;
		}	
	}

	//Moves the player in the given direction;
	if (key_left) { x += 0.1f; }
	if (key_right) { x -= 0.1f; }
	if (key_up) { y += 0.1f; }
	if (key_down) { y -= 0.1f; }
}

void MainGame::drawGameWindow() 
{
	_Display.clearGameDisplay(0.0f, 0.0f, 0.0f, 1.0f);

	//Supplies the textures that are going to be used in the program.
	Texture texture1("..\\res\\bricks.jpg");
	Texture texture2("..\\res\\water.jpg");

	//Monkey - Sets the positoin,rotation and scale for the monkey mesh.
	transform.SetPosition(glm::vec3(0.0, 0.0, -sinf(counter) * 20));
	transform.SetRotation(glm::vec3(0.0, counter * 2, 0.0));
	transform.SetScale(glm::vec3(1.0, 1.0, 1.0));

	//Binds the mesh to the GPU and draws the mesh to the display.
	shader.BindToGPU();
	shader.Update(transform, mainCamera);
	texture1.BindTexture(0);
	mesh1.drawMesh();
	mesh1.updateSphereData(*transform.GetPosition(), 1.0f);

	//Apple - Sets the positoin,rotation and scale for the apple mesh.
	transform.SetPosition(glm::vec3(0.0, -sinf(counter) * 20, 0.0));
	transform.SetRotation(glm::vec3(0.0, counter * 2, 0.0));
	transform.SetScale(glm::vec3(0.5, 0.5, 0.5));

	//Binds the mesh to the GPU and draws the mesh to the display.
	shader.BindToGPU();
	shader.Update(transform, mainCamera);
	texture2.BindTexture(0);
	mesh2.drawMesh();
	mesh2.updateSphereData(*transform.GetPosition(), 1.0f);

	//Dog - Sets the positoin,rotation and scale for the dog mesh.
	transform.SetPosition(glm::vec3(x, y, 0.0));
	transform.SetRotation(glm::vec3(0.0, counter * 2, 0.0));
	transform.SetScale(glm::vec3(0.5, 0.5, 0.5));

	//Binds the mesh to the GPU and draws the mesh to the display.
	shader.BindToGPU();
	shader.Update(transform, mainCamera);
	texture2.BindTexture(0);
	mesh3.drawMesh();
	mesh3.updateSphereData(*transform.GetPosition(), 1.0f);


	counter = counter + 0.05f;

	glEnableClientState(GL_COLOR_ARRAY);
	glEnd();

	_Display.swapBuffer();
}

//Manges collisions between meshes and updates the console.
bool MainGame::objectCollision(glm::vec3 obj1Pos, float obj1Rad, glm::vec3 obj2Pos, float obj2Rad, string names)
{
	float distance = ((obj2Pos.x - obj1Pos.x) * (obj2Pos.x - obj1Pos.x) + (obj2Pos.y - obj1Pos.y) * (obj2Pos.y - obj1Pos.y) + (obj2Pos.z - obj1Pos.z) * (obj2Pos.z - obj1Pos.z));

	if (distance * distance < (obj1Rad + obj2Rad))
	{
		cout << "Collision Between " << names << ", Distance: " << distance << '\n';
		return true;
	}
	else
	{
		return false;
	}
}

