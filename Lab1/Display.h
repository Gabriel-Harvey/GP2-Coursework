#pragma once
#include <SDL/SDL.h>
#include <GL\glew.h>
#include <iostream>
#include <string>
using namespace std;


class Display
{
public:
	Display(); //Sets the screen width and height values. 
	~Display(); //Destroys the window components.
	void initDisplay(); //Initialises the display components.
	void swapBuffer(); //Swaps the buffer for the game window.
	void clearGameDisplay(float r, float g, float b, float a); //Cleans the games display.

	float getScreenWidth(); //Grabs screen width.
	float getScreenHeight(); //Grabs screen height.

private:

	void returnError(std::string errorString); //Displays an error message if there is a fault.

	SDL_GLContext sdlGlContext;
	SDL_Window* sdlGameWindow;
	int _screenWidth;
	int _screenHeight;
};
