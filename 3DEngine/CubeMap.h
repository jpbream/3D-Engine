#pragma once
#include "Entity.h"

class CubeMap : public Entity
{
public:
	CubeMap(Model* model);
	~CubeMap();

	Texture* PosX;
	Texture* NegX;

	Texture* PosY;
	Texture* NegY;
	
	Texture* PosZ;
	Texture* NegZ;

	void Sample(const Vec4& direction, Color* out);
};

