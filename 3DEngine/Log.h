#pragma once

namespace Log {

	enum Level {
		Carefree,
		Skeptical,
		Cautious
	};
	
	extern Level CautionLevel;

	void Message(const char* s);
	void Warn(const char* s);
	void Error(const char* s);
	void RuntimeError(const char* s);
	void Assert(bool condition, const char* s);

}
