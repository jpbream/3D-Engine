#pragma once
#include "Vertex.h"
#include "Attribute.h"

enum TriangleType {
	FlatBottom,
	FlatTop
};

class Interpolator
{

private:
	
	Vertex left;
	Vertex right;

	//Attribute x;
	//Attribute z;

	//Attribute xWorld;
	//Attribute yWorld;
	//Attribute zWorld;

	//Attribute inverseW;

	//Attribute uPersp;
	//Attribute vPersp;

	//Attribute nx;
	//Attribute ny;
	//Attribute nz;

public:
	Interpolator(Vertex* left, Vertex* right, Vertex* top, TriangleType tt);

	double YTop;
	double YBottom;

	double X1;
	double X2;

	int MaxX;

	double Z_NDC;

	Vertex pixelVertex;

	void MoveDown();
	void MoveAcross();
};

