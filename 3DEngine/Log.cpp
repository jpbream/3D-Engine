#include <iostream>
#include "Log.h"

namespace Log {

	Level CautionLevel = Level::Cautious;

	void Message(const char* s) {
		if (CautionLevel != Carefree) {
			std::cout << "Message: " << s << std::endl;
		}
	}

	void Warn(const char* s) {
		if (CautionLevel != Carefree) {
			std::cout << "Warning! " << s << std::endl;
		}
	}

	void Error(const char* s) {
		if (CautionLevel == Cautious) {
			std::cout << "Error! " << s << std::endl;
		}
	}

	void RuntimeError(const char* s) {
		std::cout << "Error! " << s << std::endl;
		std::cout << "It is necessary to terminate the program!" << std::endl;
		exit(-1);
	}

	void Assert(bool condition, const char* s) {
		if (!condition) {
			RuntimeError(s);
		}
	}
}