#include "Timer.h"
#include <chrono>
#include <iostream>

void Timer::start()
{
	m_Start = std::chrono::high_resolution_clock::now();
}

void Timer::end()
{
	std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
	m_Duration = end - m_Start;
}

void Timer::printMs()
{
	float ms = m_Duration.count() * 1000.0f;
	std::cout << ms << " ms\n";
}

float Timer::getDeltaTime()
{
	return m_Duration.count();
}
