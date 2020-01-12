#pragma once
#include <vector>
#include <string>

using namespace std;
class Texture;

namespace TextureLibrary {

	static vector<string> filenames;

	void InitNewTexture(string filename, Texture* tex);
	void Load(Texture* tex);
	void Unload(Texture* tex);

}