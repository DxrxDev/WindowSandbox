#include<thread>
#include"Timer.h"

Timer::Timer() {
	last = std::chrono::steady_clock::now();
}

float Timer::Mark() {
	std::chrono::steady_clock::time_point old = last;
	last = std::chrono::steady_clock::now();
	std::chrono::duration<float> time = last - old;
	return time.count();
}

float Timer::Peek() const{
	return std::chrono::duration<float>(std::chrono::steady_clock::now() - last).count();
}

void Timer::Wait(int ms) const{
	std::this_thread::sleep_for(std::chrono::microseconds(ms));
}