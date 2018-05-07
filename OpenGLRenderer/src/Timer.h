#pragma once

#include <chrono>

class Timer
{
private:
	std::chrono::high_resolution_clock::time_point m_Start;
	std::chrono::duration<float> m_Duration;
public:
	void start();
	void end();
	void printMs();

	// Returns time in seconds
	float getDeltaTime();
};

