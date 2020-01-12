#include "IO.h"
#include <iostream>

void IO::RemoveFile(std::string path) {
	remove(path.c_str());
	std::cout << "Deleting File: " << path << std::endl;
}