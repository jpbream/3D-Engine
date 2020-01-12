#include "Model.h"
#include "Log.h"
#include "Face.h"

Vec4 noDataPosition(0, 0, 0, 1);
Vec2 noDataTexel(0, 0);
Vec4 noDataNormal(0, 0, 0, 0);
Material noDataMaterial;

Model::Model(int numPositions, int numTexCoords, int numNormals, int numFaces, int numMaterials) : 
	numPositions(numPositions), numTexCoords(numTexCoords), numNormals(numNormals), numFaces(numFaces), numMaterials(numMaterials) {

	positions = new Vec4[numPositions];
	texels = new Vec2[numTexCoords];
	normals = new Vec4[numNormals];

	tangents = new Vec4[numPositions];

	faces = new Face[numFaces];
	materials = new Material[numMaterials];

	int allocatedBytes = sizeof(Vec4) * numPositions + sizeof(Vec2) * numTexCoords + sizeof(Vec4) * numNormals + sizeof(Face) * numFaces
		+ sizeof(Material) * numMaterials + sizeof(Vec4) * numPositions;

	std::cout << "Allocating " << allocatedBytes / 1000000.0 << " megabytes for model" << std::endl;

}

Model::Model(Model&& m) : 
	positions(m.positions), texels(m.texels), normals(m.normals), tangents(m.tangents), faces(m.faces), materials(m.materials)
{
	numPositions = m.numPositions;
	numTexCoords = m.numTexCoords;
	numNormals = m.numNormals;
	numNormals = m.numNormals;
	numFaces = m.numFaces;
	numMaterials = m.numMaterials;

	m.faces = nullptr;
	m.positions = nullptr;
	m.texels = nullptr;
	m.normals = nullptr;
	m.materials = nullptr;
	m.tangents = nullptr;
}

/*void Model::LoadResources() {
	for (int i = 0; i < numMaterials; ++i) {
		materials[i].Load();
	}
	loaded = true;
}

void Model::UnloadResources() {
	for (int i = 0; i < numMaterials; ++i) {
		materials[i].Unload();
	}
	loaded = false;
}*/

Model::~Model() {
	delete[] faces;

	delete[] positions;
	delete[] texels;
	delete[] normals;

	delete[] tangents;

	/*if (materials != nullptr) {
		UnloadResources();
	}*/
	delete[] materials;
}