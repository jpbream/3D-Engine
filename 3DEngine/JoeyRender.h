#pragma once
#include <SDL.h>
#include "IO.h"

void JoeyRenderInit() {

	SDL_Init(SDL_INIT_VIDEO);
	IO::InitGdiPlus();

}

void JoeyRenderQuit() {

	SDL_Quit();
	IO::UnInitGdiPlus();

}