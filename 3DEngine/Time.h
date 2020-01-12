#pragma once
#include <chrono>

static std::chrono::time_point<std::chrono::high_resolution_clock> t1;
static std::chrono::time_point<std::chrono::high_resolution_clock> t2;

namespace Timer {
	void start() {
		t1 = std::chrono::high_resolution_clock::now();
	}
	void stop() {
		t2 = std::chrono::high_resolution_clock::now();
	}
	double duration() {
		std::chrono::duration<double> duration = t2 - t1;
		return duration.count();
	}
}



