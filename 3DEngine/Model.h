#pragma once
#include "Vec4.h"
#include "Vec2.h"
#include <string>
#include "Material.h"

class Face;

const int NO_VALUE = -123456789;
extern Vec4 noDataPosition;
extern Vec2 noDataTexel;
extern Vec4 noDataNormal;
extern Material noDataMaterial;

class Model {

	typedef std::string string;

private:
	//bool loaded = false;

public:

	Model(int numPositions, int numTexCoords, int numNormals, int numFaces, int numMaterials);
	Model(Model&&);
	~Model();

	//void LoadResources();
	//void UnloadResources();
	//bool IsLoaded() {
		//return loaded;
	//}

	int numPositions;
	int numTexCoords;
	int numNormals;

	int numFaces;
	int numMaterials;

	Vec4* positions;
	Vec2* texels;
	Vec4* normals;

	Vec4* tangents;

	Face* faces;
	Material* materials;

};
