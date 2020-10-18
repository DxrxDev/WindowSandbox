#pragma once
#include<chrono>

class Timer {
public:
	Timer();
	float Mark();
	float Peek() const;
	void Wait(int ms) const;
private:
	std::chrono::steady_clock::time_point last;
};