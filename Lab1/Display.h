#pragma once
#include <SDL/SDL.h>
#include <GL\glew.h>
#include <iostream>
#include <string>
using namespace std;


class Display
{
public:
	Display();
	~Display();
	void initDisplay();
	void swapBuffer();
	void clearGameDisplay(float r, float g, float b, float a);

	float getScreenWidth();
	float getScreenHeight();

private:

	void returnError(std::string errorString);

	SDL_GLContext sdlGlContext;
	SDL_Window* sdlGameWindow;
	int _screenWidth;
	int _screenHeight;
};
