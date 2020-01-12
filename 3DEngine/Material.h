#pragma once
#include "Color.h"
#include "Texture.h"

class Material
{

	friend class Model;

private:
	static void TransferCopyables(Material& dest, const Material& src);
	
public:
	Material();
	//~Material();

	Material(const Material&);
	Material& operator=(const Material& m) = delete;

	void CopyFrom(const Material& m);
	void MoveFrom(Material& m);

	void CopyTo(Material& m);
	void MoveTo(Material& m);

	Color ambientColor = Color(0, 0, 0);
	Texture ambientTexture;

	Color diffuseColor = Color(1, 1, 1);
	Texture diffuseTexture;

	Texture normalMap;

	Color specularColor = Color(1, 0, 1);
	double specularExponent = 0;

	Color emissionColor = Color(0, 0, 0);

	//will contain many more textures and instance variables like specular color etc.
	//need to remember to load and unload any new maps that I add
};

