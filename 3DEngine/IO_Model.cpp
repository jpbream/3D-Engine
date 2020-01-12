#include "IO.h"
#include <fstream>
#include <iostream>
#include "Vec2.h"
#include "ModelOperations.h"
#include "Face.h"

using namespace std;

static int GetFaceInfo(string& s) {

	if (s.length() == 0) {
		return NO_VALUE + 1;
	}

	int index = s.find('/');

	if (index == 0) {
		s.replace(0, index + 1, "");
		return NO_VALUE + 1;
	}

	int i;
	if (index >= 0) {
		i = stoi(s.substr(0, index));
	}
	else {
		i = stoi(s);
		s = "";
	}
	s.replace(0, index + 1, "");
	return i;
}

static string& operator>>(string& s, string& i) {

	int index = s.find(' ');
	if (index >= 0) {
		i = s.substr(0, index);
		s.replace(0, index + 1, "");
	}
	else {
		i = s;
	}

	return s;
}

static string ExtractType(string& s) {
	int index = s.find(' ');
	string type = s.substr(0, index);
	s.replace(0, index + 1, "");
	return type;
}

 void IO::LoadMtl(string path, vector<Material>& materials, map<string, int>& namesMap) {

	if (path.substr(path.length() - 4, path.length()) == ".mtl") {

		ifstream fin(path);
		string line;

		int editingMaterial = -1;

		while (fin) {
			getline(fin, line);
			string lineType = ExtractType(line);

			if (lineType == "newmtl") {
				materials.push_back(Material());
				//materials.emplace_back(Material());
				editingMaterial = materials.size() - 1;

				string materialName;
				line >> materialName;
				std::cout << "Loaded Material: " << materialName << std::endl;

				namesMap.emplace(materialName, editingMaterial);
			}

			//ambient color
			if (lineType == "Ka") {
				string rNorm, gNorm, bNorm;
				line >> rNorm >> gNorm >> bNorm;

				double r = stod(rNorm);
				double g = stod(gNorm);
				double b = stod(bNorm);

				materials[editingMaterial].ambientColor = Color(r, g, b);
			}

			//ambient map
			if (lineType == "map_Ka") {
				string file;
				line >> file;
				materials[editingMaterial].ambientTexture.CreateFromFile(file);
			}

			//diffuse color
			if (lineType == "Kd") {
				string rNorm, gNorm, bNorm;
				line >> rNorm >> gNorm >> bNorm;

				double r = stod(rNorm);
				double g = stod(gNorm);
				double b = stod(bNorm);

				materials[editingMaterial].diffuseColor = Color(r, g, b);
			}

			//diffuse texture map
			if (lineType == "map_Kd") {
				string file;
				line >> file;
				//TextureLibrary::InitNewTexture(file, &(materials[editingMaterial].diffuseTexture));
				//TextureLibrary::Load(&(materials[editingMaterial].diffuseTexture));
				materials[editingMaterial].diffuseTexture.CreateFromFile(file);
			}

			//normal map
			if (lineType == "norm") {
				string file;
				line >> file;
				//TextureLibrary::InitNewTexture(file, &(materials[editingMaterial].normalMap))
				materials[editingMaterial].normalMap.CreateFromFile(file);
				
			}

			//specular color
			if (lineType == "Ks") {
				string rNorm, gNorm, bNorm;
				line >> rNorm >> gNorm >> bNorm;

				double r = stod(rNorm);
				double g = stod(gNorm);
				double b = stod(bNorm);

				materials[editingMaterial].specularColor = Color(r, g, b);
			}

			//specular exponent
			if (lineType == "Ns") {
				string num;
				line >> num;

				double exp = stod(num);
				materials[editingMaterial].specularExponent = exp;
			}

			//emission color
			if (lineType == "Ke") {
				string rNorm, gNorm, bNorm;
				line >> rNorm >> gNorm >> bNorm;

				double r = stod(rNorm);
				double g = stod(gNorm);
				double b = stod(bNorm);

				materials[editingMaterial].emissionColor = Color(r, g, b);
			}
		}
	
		fin.close();
	}
}

Model IO::LoadObj(string path) {

	if (path.substr(path.length() - 4, path.length()) == ".obj") {

		ifstream fin(path);
		string line;

		vector<Vec4> positions;
		vector<Vec2> texels;
		vector<Vec4> normals;

		vector<Face> faces;

		vector<Material> materials;
		map<string, int> namesMap;
		int activeMaterialIndex = NO_VALUE;

		while (fin) {

			getline(fin, line);

			string lineType = ExtractType(line);

			if (lineType == "mtllib") {
				LoadMtl(line, materials, namesMap);
			}

			if (lineType == "usemtl") {
				string materialName;
				line >> materialName;

				if (namesMap.find(materialName) != namesMap.end()) {
					activeMaterialIndex = namesMap.at(materialName);
				}
				else {
					activeMaterialIndex = NO_VALUE;
				}
			}

			if (lineType == "v") {

				Vec4 vertex;

				string x, y, z;

				line >> x >> y >> z;
				vertex.x = stod(x);
				vertex.y = stod(y);
				vertex.z = stod(z);
				vertex.w = 1;

				line = "";
				positions.push_back(vertex);
			}

			if (lineType == "vt") {
				Vec2 texel;

				string u, v;

				line >> u >> v;
				texel.x = stod(u);
				texel.y = stod(v);

				line = "";
				texels.push_back(texel);
			}

			if (lineType == "vn") {

				Vec4 normal;
				string x, y, z;

				line >> x >> y >> z;
				normal.x = stod(x);
				normal.y = stod(y);
				normal.z = stod(z);
				normal.w = 0;

				normal.Normalize();

				line = "";
				normals.push_back(normal);

			}

			if (lineType == "f") {

				//	v/vt/vn

				string f1, f2, f3;
				line >> f1 >> f2 >> f3;

				//this does not handle the possibility of some faces having some points with texture coordinates
				//and some without

				int p1 = -1, p2 = -1, p3 = -1;
				int t1 = -1, t2 = -1, t3 = -1;
				int n1 = -1, n2 = -1, n3 = -1;
				
				//gets the positions of each vertex
				p1 = GetFaceInfo(f1);
				p2 = GetFaceInfo(f2);
				p3 = GetFaceInfo(f3);

				//gets the texture coords of each vertex
				t1 = GetFaceInfo(f1);
				t2 = GetFaceInfo(f2);
				t3 = GetFaceInfo(f3);

				//gets the normals of each vertex
				n1 = GetFaceInfo(f1);
				n2 = GetFaceInfo(f2);
				n3 = GetFaceInfo(f3);

				p1--; t1--; n1--; p2--; t2--; n2--; p3--; t3--; n3--;

				Face face(Index(p1, t1, n1), Index(p2, t2, n2), Index(p3, t3, n3));
				face.materialID = activeMaterialIndex;

				faces.push_back(face);
				
			}

		}

		fin.close();

		int numPositions = positions.size();
		int numTexCoords = texels.size();
		int numNormals = normals.size();

		int numFaces = faces.size();
		int numMaterials = materials.size();

		std::cout << "Loaded " << numPositions << " positions" << std::endl;
		std::cout << "Loaded " << numTexCoords << " texels" << std::endl;
		std::cout << "Loaded " << numNormals << " normals" << std::endl;
		std::cout << "Loaded " << numFaces << " faces" << std::endl;
		std::cout << "Loaded " << numMaterials << " materials" << std::endl;

		Model m(numPositions, numTexCoords, numNormals, numFaces, numMaterials);

		for (int i = 0; i < positions.size(); ++i) {
			m.positions[i] = positions[i];
		}

		for (int i = 0; i < texels.size(); ++i) {
			m.texels[i] = texels[i];
		}

		for (int i = 0; i < normals.size(); ++i) {
			m.normals[i] = normals[i];
		}

		for (int i = 0; i < numFaces; ++i) {
			m.faces[i] = faces[i];
		}

		bool hasLoadedTangents = false;
		for (int i = 0; i < numMaterials; ++i) {

			m.materials[i].MoveFrom(materials[i]);
			
			if (m.materials[i].normalMap.IsNull() == false && hasLoadedTangents == false) {

				ModelOperations::CalculateTangents(m);
				hasLoadedTangents = true;
			}
		}

		positions.clear();
		texels.clear();
		normals.clear();
		faces.clear();
		materials.clear();

		return m;

	}

	return Model(0, 0, 0, 0, 0);
	
}
