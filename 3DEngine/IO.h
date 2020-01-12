#pragma once
#include "Model.h"
#include <string>
#include <vector>
#include <map>

class IO {

	friend void JoeyRenderInit();
	friend void JoeyRenderQuit();

private:

	static void InitGdiPlus();
	static void UnInitGdiPlus();

public:
	static Model LoadObj(std::string path);
	static void LoadMtl(std::string path, std::vector<Material>& materials, std::map<std::string, int>& namesMap);

	static void CreateBMPFrom(std::string& badFile);

	static void RemoveFile(std::string name);

};

