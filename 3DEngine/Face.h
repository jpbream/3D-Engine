#pragma once
#include "Index.h"

class Face {

public:

	Face();
	Face(const Index&, const Index&, const Index&);
	Face& operator=(const Face&);

	Index index1;
	Index index2;
	Index index3;

	int materialID;

	Vec4 normal;

};
