#pragma once
#include <SDL.h>
#include "Color.h"
#include "Window.h"
#include "Log.h"
#include <iostream>
#include "Texture.h"

class Surface {

	friend class Window;
	friend class Renderer;

public:

	Surface(int width, int height);
	~Surface();

	int GetWidth() { return width; }
	int GetHeight() { return height; }
	int NumPixels() { return width * height; }

	void CopyToWindow(Window* w);
	void CopyToTexture(Texture* t, int startX, int startY);
	void SaveToFile(std::string filename);

	void ClearBuffer();
	void Fill(Color& c);

	inline void PutPixel(int x, int y, Color& c) {
		buffer[y * width + x] = c.Compress();
	}

	void DrawLine(int x1, int y1, int x2, int y2, Color& c);
	void FillRect(int x, int y, int w, int h, Color& c);
	void DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, Color& c);
	

private:

	static const int BITS_PER_PIXEL = 32;
	static const int BYTES_PER_PIXEL = 4;

	static const int RMASK = 0xff000000;
	static const int GMASK = 0x00ff0000;
	static const int BMASK = 0x0000ff00;
	static const int AMASK = 0x000000ff;

	int width;
	int height;
	int pitch;

	int* buffer;
	int bufferSize;

};

