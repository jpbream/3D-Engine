#pragma once
#include <string>

namespace BMPTracker {

	void InitGDIPlus();
	void CreateBMPFrom(std::string& badImage);
	void UnInit();

}

