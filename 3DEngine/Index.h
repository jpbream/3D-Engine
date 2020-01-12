#pragma once
#include "Vec4.h"
#include "IO.h"

enum Buffer : int;

struct Index {

	friend class Entity;
	friend class Model;
	friend class ModelOperations;

private:
	int positionIndex;
	int textureIndex;
	int normalIndex;

public:

	Index();
	Index(Buffer);
	Index(int, int, int);
	Index& operator=(const Index&);

};
