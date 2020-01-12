#include <memory>
#include <iostream>
#include "Vec2.h"
#include "Surface.h"


Surface::Surface(int width, int height) : width(width), height(height) {

	pitch = BYTES_PER_PIXEL * width;

	bufferSize = height * pitch;
	std::cout << "Allocating " << bufferSize / 1000000.0 << " megabytes for Pixel Buffer" << std::endl;

	buffer = new int[width * height];
	
}

Surface::~Surface() {
	delete[] buffer;
}

void Surface::CopyToWindow(Window* w) {

	SDL_Surface* surface = SDL_CreateRGBSurfaceFrom((void*)buffer, width, height, BITS_PER_PIXEL, pitch, RMASK, GMASK, BMASK, AMASK);

	SDL_Texture* texture = SDL_CreateTextureFromSurface(w->renderer, surface);
	SDL_FreeSurface(surface);

	SDL_RenderClear(w->renderer);
	SDL_RenderCopy(w->renderer, texture, NULL, NULL);
	SDL_RenderPresent(w->renderer);
	SDL_DestroyTexture(texture);
}

void Surface::CopyToTexture(Texture* t, int startX, int startY) {
	

	int startLocation = (int)buffer + startY * width * sizeof(int) + startX * sizeof(int);
	int amountToCopy = t->GetSize();

	if (startLocation + amountToCopy > (int)buffer + bufferSize) {
		amountToCopy = (int)buffer + bufferSize - startLocation;
	}

	memcpy(t->texture, buffer, amountToCopy);

	t->bottomUp = false;
	t->bottomUp = true;
	t->readBackwards = false;
	t->bpp = 32;
	t->pitch = t->width * 32 / 8;

}

void Surface::SaveToFile(std::string filename) {

	SDL_Surface* surface = SDL_CreateRGBSurfaceFrom((void*)buffer, width, height, BITS_PER_PIXEL, pitch, RMASK, GMASK, BMASK, AMASK);
	SDL_SaveBMP(surface, filename.c_str());
	SDL_FreeSurface(surface);

}

void Surface::ClearBuffer() {
	memset(buffer, 0, bufferSize);
}

void Surface::DrawLine(int x1, int y1, int x2, int y2, Color& c) {

	int dx = x2 - x1;
	int dy = y2 - y1;

	if (dx == 0 && dy == 0) {
		return;
	}

	if (abs(dy) > abs(dx)) {

		double slope = (double)dx / dy;

		int inc = (y2 - y1) / abs(y2 - y1);

		for (int y = y1; y != y2; y += inc) {
			int x = (int)(x1 + (y - y1) * slope);

			PutPixel(x, y, c);
		}

	}
	else {

		double slope = (double)dy / dx;

		int inc = (x2 - x1) / abs(x2 - x1);

		for (int x = x1; x != x2; x += inc) {
			int y = (int)(y1 + (x - x1) * slope);

			PutPixel(x, y, c);
		}

	}

}

void Surface::FillRect(int x, int y, int w, int h, Color& c) {

	for (int i = 0; i < h; ++i) {
		for (int t = x; t < x + w; ++t) {
			buffer[(y + i) * width + t] = c.Compress();
		}
	}

}

void Surface::DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, Color& c) {
	DrawLine(x1, y1, x2, y2, c);
	DrawLine(x1, y1, x3, y3, c);
	DrawLine(x2, y2, x3, y3, c);
}

void Surface::Fill(Color& c) {
	for (int i = 0; i < width; ++i) {
		for (int j = 0; j < height; ++j) {
			PutPixel(i, j, c);
		}
	}
}

