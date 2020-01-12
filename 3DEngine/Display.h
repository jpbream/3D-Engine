#pragma once
#include <SDL.h>

namespace Display {

	int GetWidth() {
		SDL_DisplayMode DM;
		SDL_GetCurrentDisplayMode(0, &DM);
		return DM.w;
	}

	int GetHeight() {
		SDL_DisplayMode DM;
		SDL_GetCurrentDisplayMode(0, &DM);
		return DM.h;
	}

	double GetAspectRatio() {
		return (int)GetWidth() / GetHeight();
	}

	int GetWeightedWidth(double proportion) {
		return (int)(GetWidth() * proportion);
	}

	int GetWeightedHeight(double proportion) {
		return (int)(GetHeight() * proportion);
	}
}
