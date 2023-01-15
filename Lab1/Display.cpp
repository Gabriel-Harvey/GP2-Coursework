#include "Display.h"

//Sets the screen width and height values. 
Display::Display()
{
	sdlGameWindow = nullptr;
	_screenWidth = 1024;
	_screenHeight = 768;
}

//Destroys the window components.
Display::~Display()
{
	SDL_GL_DeleteContext(sdlGlContext);
	SDL_DestroyWindow(sdlGameWindow);
	SDL_Quit();
}

//Grabs the screen width and hieght.
float Display::getScreenWidth() { return _screenWidth; }
float Display::getScreenHeight() { return _screenHeight; }

//Displays an error message if there is a fault.
void Display::returnError(std::string errorString)
{
	std::cout << errorString << std::endl;
	std::cout << "press any key to quit...";
	int in;
	std::cin >> in;
	SDL_Quit();
}

//Swaps the buffer for the game window.
void Display::swapBuffer()
{
	SDL_GL_SwapWindow(sdlGameWindow);
}

//Cleans the game display.
void Display::clearGameDisplay(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

//Initialises the display components.
void Display::initDisplay()
{
	SDL_Init(SDL_INIT_EVERYTHING);

	//Sets the display colour values, depth size and creates a double buffer.
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	
	//Creates the game window and assigns a name an other values.
	sdlGameWindow = SDL_CreateWindow("Game Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, (int)_screenWidth, (int)_screenHeight, SDL_WINDOW_OPENGL);

	//Retursn an error if the game window failed to create.
	if (sdlGameWindow == nullptr)
	{
		returnError("Window has failed to be created");
	}

	sdlGlContext = SDL_GL_CreateContext(sdlGameWindow);

	//Returns an error if the the SDL_GL context failed to create.
	if (sdlGlContext == nullptr)
	{
		returnError("SDL_GL context has failed to be created");
	}

	GLenum glError = glewInit();

	//Returns an error if the GLEW failed to create.
	if (glError != GLEW_OK)
	{
		returnError("GLEW has failed to initialise");
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glClearColor(0.f, 1.0f, 1.0f, 1.0f);
}