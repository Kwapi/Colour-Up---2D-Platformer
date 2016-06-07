#pragma once

#include <ctime>

class Clock {

public:
	Clock() { }

	void start() {
		startedAt = clock();
	}

	double restart() {
		double timeElapsed = stop();
		start();
		return timeElapsed;
	}

	double stop() {
		double timeElapsed = (clock() - startedAt) / double(CLOCKS_PER_SEC);
		return timeElapsed;
	}

private:
	clock_t startedAt;
};

