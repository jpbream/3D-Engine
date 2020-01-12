#include <iostream>
#include "Window.h"

Window::Window(const char* title, int x, int y, int w, int h, bool fullscreen) {

	window = SDL_CreateWindow(title, x, y, w, h, fullscreen ? SDL_WINDOW_FULLSCREEN : 0);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

}

Window::~Window() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
}

void Window::SetText(const char* name) {
	SDL_SetWindowTitle(window, name);
}

void Window::Update() {
	SDL_UpdateWindowSurface(window);
}

void Window::ShowImage(std::string filename) {

	SDL_Surface* image = SDL_LoadBMP(filename.c_str());

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, image);
	SDL_FreeSurface(image);

	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, texture, NULL, NULL);
	SDL_RenderPresent(renderer);
	SDL_DestroyTexture(texture);

}

const char* Window::GetText() {
	return SDL_GetWindowTitle(window);
}

int Window::GetWidth() {
	int w;
	SDL_GetWindowSize(window, &w, nullptr);
	return w;
}

int Window::GetHeight() {
	int h;
	SDL_GetWindowSize(window, nullptr, &h);
	return h;
}