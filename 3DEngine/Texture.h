#pragma once
#include "Color.h"
#include <string>
#include <iostream>
//#include "TextureLibrary.h"

class Texture {

	friend class Surface;
	//friend class Material;
	//friend class Surface;
	//friend void TextureLibrary::InitNewTexture(std::string, Texture*);
	//friend void TextureLibrary::Load(Texture*);
	//friend void TextureLibrary::Unload(Texture*);

private:

	//int ID = -1;
	unsigned char* texture = nullptr;
	int width;
	int height;
	bool bottomUp;
	bool readBackwards;
	int pitch;
	int bpp;

	int GetSize();

public:

	Texture();
	Texture(int width, int height);

	Texture(Texture&& t);
	Texture(std::string filename);

	~Texture();

	Texture(Texture& t) = delete;
	Texture& operator=(const Texture& t) = delete;

	void CopyFrom(const Texture& t);
	void MoveFrom(Texture& t);

	void CopyTo(Texture& t);
	void MoveTo(Texture& t);

	void CreateFromFile(std::string filename);

	void Free();

	void Sample(double u, double v, Color* c) const;

	bool IsNull() const {
		return texture == nullptr;
	}

	int GetWidth() const {
		return width;
	}

	int GetHeight() const {
		return height;
	}

};
