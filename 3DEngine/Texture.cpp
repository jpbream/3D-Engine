#include "Texture.h"
#include <atlimage.h>
#include <iostream>
#include "IO.h"
#include <atlstr.h>
#include <atlimage.h>

Texture::Texture() {

	texture = nullptr;

	width = 0;
	height = 0;
	bottomUp = false;
	readBackwards = false;
	pitch = 0;
	bpp = 0;

}

Texture::Texture(int width, int height) : width(width), height(height) {

	bottomUp = false;
	readBackwards = false;
	bpp = 32;
	pitch = width * bpp / 8;

	texture = (unsigned char*)malloc(height * pitch);

}

Texture::Texture(Texture&& t) : texture(t.texture) {

	std::cout << "Moving Texture" << std::endl;

	width = t.width;
	height = t.height;
	bottomUp = t.bottomUp;
	readBackwards = t.readBackwards;
	pitch = t.pitch;
	bpp = t.bpp;

	t.texture = nullptr;

}

Texture::Texture(std::string filename) {
	CreateFromFile(filename);
}

void Texture::CopyFrom(const Texture& t) {

	if (this != &t && !t.IsNull()) {


		delete[] texture;
		int size = t.height * t.pitch;
		texture = (unsigned char*)malloc(size);

		memcpy(texture, t.texture, size);

		std::cout << "Copying Texture : " << size / 1000000.0 << " megabytes" << std::endl;

		width = t.width;
		height = t.height;
		bottomUp = t.bottomUp;
		readBackwards = t.readBackwards;
		pitch = t.pitch;
		bpp = t.bpp;

	}

}

void Texture::MoveFrom(Texture& t) {

	if (this != &t && !t.IsNull()) {

		width = t.width;
		height = t.height;
		bottomUp = t.bottomUp;
		readBackwards = t.readBackwards;
		pitch = t.pitch;
		bpp = t.bpp;

		t.width = 0;
		t.height = 0;
		t.bottomUp = false;
		t.readBackwards = false;
		t.pitch = 0;
		t.bpp = 0;

		texture = t.texture;
		t.texture = nullptr;
		
		std::cout << "Moving Texture" << std::endl;

	}

}

void Texture::CopyTo(Texture& t) {

	if (this != &t && !IsNull()) {


		delete[] t.texture;
		int size = height * pitch;
		t.texture = (unsigned char*)malloc(size);

		memcpy(t.texture, texture, size);

		std::cout << "Copying Texture : " << size / 1000000.0 << " megabytes" << std::endl;

		t.width = width;
		t.height = height;
		t.bottomUp = bottomUp;
		t.readBackwards = readBackwards;
		t.pitch = pitch;
		t.bpp = bpp;

	}

}

void Texture::MoveTo(Texture& t) {

	if (this != &t && !IsNull()) {

		t.width = width;
		t.height = height;
		t.bottomUp = bottomUp;
		t.readBackwards = readBackwards;
		t.pitch = pitch;
		t.bpp = bpp;

		width = 0;
		height = 0;
		bottomUp = false;
		readBackwards = false;
		pitch = 0;
		bpp = 0;

		t.texture = texture;
		texture = nullptr;

		std::cout << "Moving Texture" << std::endl;

	}
}

void Texture::CreateFromFile(std::string filename) {

	std::string filetype = filename.substr(filename.find("."), filename.size());

	bool createdFile = false;
	if (filetype != ".bmp") {

		IO::CreateBMPFrom(filename);
		createdFile = true;

	}

	if (texture != nullptr) {
		return;
	}

	CImage cimg;
	HRESULT h = cimg.Load(filename.c_str());

	//std::cout << (unsigned long)h << " result\n";

	width = cimg.GetWidth();
	height = cimg.GetHeight();

	int pitch = cimg.GetPitch();
	if (pitch < 0) {
		bottomUp = true;
		pitch *= -1;
	}
	else {
		bottomUp = false;
	}

	readBackwards = true;

	this->pitch = pitch;
	bpp = cimg.GetBPP();

	int size = height * pitch;
	unsigned char* start = nullptr;

	if (bottomUp) {
		start = (unsigned char*)cimg.GetPixelAddress(0, height - 1);
	}
	else {
		start = (unsigned char*)cimg.GetPixelAddress(0, 0);
	}

	std::cout << "Allocating " << size / 1000000.0 << " megabytes for texture" << std::endl;
	texture = (unsigned char*)malloc(size);
	memcpy(texture, start, size);

	cimg.Destroy();

	if (createdFile) {
		IO::RemoveFile(filename);
	}

}

void Texture::Free() {

	std::cout << "Freeing Texture : " << GetSize() / 1000000.0 << " megabytes" << std::endl;

	width = 0;
	height = 0;
	bottomUp = false;
	readBackwards = false;
	pitch = 0;
	bpp = 0;

	if (texture != nullptr) {
		delete[] texture;
	}
	texture = nullptr;
	
}

int Texture::GetSize() {
	return height * pitch;
}

void Texture::Sample(double u, double v, Color* c) const {

	int x = (int)(u * width);
	int y = (int)(v * height);
	
	if (bpp < 24 || x < 0 || x >= width || y < 0 || y >= height || texture == nullptr) {
		return;
	}

	int location = 0;
	if (bottomUp) {
		location = pitch * (height - 1 - y) + (bpp / 8) * x;
	}
	else {
		location = pitch * y + (bpp / 8) * x;
	}

	if (readBackwards) {
		c->b = (unsigned int)texture[location] / 255.0;
		c->g = (unsigned int)texture[location + 1] / 255.0;
		c->r = (unsigned int)texture[location + 2] / 255.0;

		if (bpp == 32) {
			c->a = (unsigned int)texture[location + 3] / 255.0;
		}

	}
	else {

		c->a = (unsigned int)texture[location] / 255.0;
		c->b = (unsigned int)texture[location + 1] / 255.0;
		c->g = (unsigned int)texture[location + 2] / 255.0;
		c->r = (unsigned int)texture[location + 3] / 255.0;
	}

}

Texture::~Texture() {
	
	delete[] texture;
}