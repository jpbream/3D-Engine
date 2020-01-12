#pragma once
#include <SDL.h>
#include <memory>
#include "Surface.h"
#include <string>

class Window {

public:

	Window& operator=(Window&) = delete;
	Window(Window&) = delete;

	Window(const char* title, int x, int y, int w, int h, bool fullscreen);
	
	~Window();

	SDL_Window* Get() { return window; }

	void SetText(const char* name);
	void Update();

	void ShowImage(std::string filename);

	const char* GetText();
	int GetWidth();
	int GetHeight();

	//friends
	friend class Surface;


private:
	SDL_Window* window;
	SDL_Renderer* renderer;

};
